#include <string.h>
#include <stdlib.h>
#include <libnewscript/vm/vm.h>
#include <libnewscript/vm/value.h>
#include <libnewscript/vm/string.h>

void nsVmInitStringTable(NsVmStringTable* tbl)
{
    tbl->size = 1;
    tbl->capacity = 16;
    tbl->data = malloc(tbl->capacity * sizeof(*tbl->data));
    tbl->length = malloc(tbl->capacity * sizeof(*tbl->length));
    tbl->interned = calloc(1, tbl->capacity / 8);

    tbl->data[0] = "";
    tbl->length[0] = 0;

    tbl->internedMapSize = 0;
    tbl->internedMapCapacity = 16;
    tbl->internedMapBuckets = calloc(tbl->internedMapCapacity, sizeof(*tbl->internedMapBuckets));
    tbl->internedMapList = malloc(tbl->internedMapCapacity * sizeof(*tbl->internedMapList));
}

void nsVmDeinitStringTable(NsVmStringTable* tbl)
{
    free(tbl->data);
    free(tbl->length);
    free(tbl->interned);
    free(tbl->internedMapBuckets);
    free(tbl->internedMapList);
}

static uint64_t hashString(const char* str, size_t length)
{
    uint64_t hash = 0xcbf29ce484222325;
    uint8_t d;

    for (size_t i = 0; i < length; ++i)
    {
        d = (uint8_t)str[i];
        hash = ((hash ^ d) * 0x100000001b3);
    }
    return hash;
}

static NsStringId getInternedString(NsVmStringTable* tbl, const char* str, size_t length)
{
    uint64_t hash = hashString(str, length);
    uint32_t slot = hash & (tbl->internedMapCapacity - 1);
    NsStringId strId;

    strId = tbl->internedMapBuckets[slot];
    while (strId)
    {
        if (tbl->length[strId] == length && memcmp(tbl->data[strId], str, length) == 0)
            return strId;
        slot = (slot + 1) & (tbl->internedMapCapacity - 1);
        strId = tbl->internedMapBuckets[slot];
    }
    return 0;
}

static void setInternedString(NsVmStringTable* tbl, NsStringId strId)
{
    size_t newCapacity;
    NsStringId tmpId;
    NsStringId tmpId2;
    uint64_t hash;
    uint32_t slot;

    if (tbl->internedMapSize >= tbl->internedMapCapacity / 2)
    {
        newCapacity = tbl->internedMapCapacity * 2;
        tbl->internedMapBuckets = calloc(newCapacity, sizeof(*tbl->internedMapBuckets));
        tbl->internedMapList = malloc(newCapacity * sizeof(*tbl->internedMapList));
        tbl->internedMapCapacity = newCapacity;

        for (size_t i = 0; i < tbl->internedMapSize; ++i)
        {
            tmpId = tbl->internedMapList[i];
            hash = hashString(tbl->data[tmpId], tbl->length[tmpId]);
            slot = hash & (tbl->internedMapCapacity - 1);

            while (tbl->internedMapBuckets[slot])
                slot = (slot + 1) & (tbl->internedMapCapacity - 1);

            tbl->internedMapBuckets[slot] = tmpId;
        }
    }

    hash = hashString(tbl->data[strId], tbl->length[strId]);
    slot = hash & (tbl->internedMapCapacity - 1);

    while (tbl->internedMapBuckets[slot])
        slot = (slot + 1) & (tbl->internedMapCapacity - 1);

    tbl->internedMapBuckets[slot] = strId;
    tbl->internedMapList[tbl->internedMapSize++] = strId;
    tbl->interned[strId / 8] |= (1 << (strId % 8));
}

NsValue nsVmCreateString(NsVirtualMachine* vm, const char* data, size_t length, int intern)
{
    NsVmStringTable* tbl;
    NsStringId strId;
    size_t newCapacity;
    char* tmp;

    tbl = &vm->tblStrings;
    strId = getInternedString(tbl, data, length);
    if (strId)
        return nsMakeString(strId);

    if (tbl->size == tbl->capacity)
    {
        newCapacity = tbl->capacity * 2;
        tbl->data = realloc(tbl->data, newCapacity * sizeof(*tbl->data));
        tbl->length = realloc(tbl->length, newCapacity * sizeof(*tbl->length));
        tbl->interned = realloc(tbl->interned, newCapacity / 8);
        memset(tbl->interned + newCapacity / 16, 0, newCapacity / 16);
        tbl->capacity = newCapacity;
    }

    strId = tbl->size++;
    tmp = malloc(length + 1);
    memcpy(tmp, data, length);
    tmp[length] = 0;
    tbl->data[strId] = tmp;
    tbl->length[strId] = length;

    if (intern)
        setInternedString(tbl, strId);

    return nsMakeString(strId);
}

NsValue nsVmCreateCString(NsVirtualMachine* vm, const char* data, int intern)
{
    return nsVmCreateString(vm, data, strlen(data), intern);
}

const char* nsVmStringData(NsVirtualMachine* vm, NsValue str)
{
    NsVmStringTable* tbl = &vm->tblStrings;
    NsStringId strId = nsValueString(str);
    return tbl->data[strId];
}
