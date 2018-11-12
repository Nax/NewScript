#include <stdlib.h>
#include <libnewscript/vm/vm.h>
#include <libnewscript/vm/object.h>

void nsVmInitObjectTable(NsVmObjectTable* tbl)
{
    tbl->size = 0;
    tbl->capacity = 16;
    tbl->objects = calloc(tbl->capacity, sizeof(*tbl->objects));
}

void nsVmDeinitObjectTable(NsVmObjectTable* tbl)
{
    free(tbl->objects);
}

NsValue nsVmCreateObject(NsVirtualMachine* vm)
{
    NsVmObjectTable* tbl;
    NsObjectId objId;
    NsObject* obj;
    size_t newCapacity;

    tbl = &vm->tblObjects;
    if (tbl->size == tbl->capacity)
    {
        newCapacity = tbl->capacity + tbl->capacity / 2;
        tbl->objects = realloc(tbl->objects, newCapacity * sizeof(*tbl->objects));
        tbl->capacity = newCapacity;
    }
    objId = tbl->size++;
    obj = tbl->objects + objId;
    obj->propSize = 0;
    obj->propCapacity = 8;
    obj->keys = malloc(obj->propCapacity * sizeof(*obj->keys));
    obj->values = malloc(obj->propCapacity * sizeof(*obj->values));
    obj->buckets = malloc(obj->propCapacity * sizeof(*obj->buckets));
    obj->bitmap = calloc(1, obj->propCapacity / 8);
    return nsMakeObject(objId);
}

static int keyCompare(NsVirtualMachine* vm, NsValue a, NsValue b)
{
    // TODO: Make a real compare
    (void)vm;
    return (a == b);
}

static uint64_t hashValue(NsValue value)
{
    // TODO: Should probably be a real hashing function
    return value;
}

static NsObject* getObject(NsVmObjectTable* tbl, NsValue value)
{
    NsObjectId objId = nsValueObject(value);
    return tbl->objects + objId;
}

void nsVmSetObjectProperty(NsVirtualMachine* vm, NsValue object, NsValue key, NsValue value)
{
    size_t newCapacity;
    NsVmObjectTable* tbl = &vm->tblObjects;
    NsObject* obj = getObject(tbl, object);

    uint64_t hash;
    uint32_t slot;
    uint32_t index;

    if (obj->propSize >= obj->propCapacity / 2)
    {
        newCapacity = obj->propCapacity * 2;
        obj->keys = realloc(obj->keys, newCapacity * sizeof(*obj->keys));
        obj->values = realloc(obj->values, newCapacity * sizeof(*obj->values));
        obj->buckets = realloc(obj->buckets, newCapacity * sizeof(*obj->buckets));
        obj->bitmap = calloc(1, newCapacity / 8);
        obj->propCapacity = newCapacity;

        for (size_t i = 0; i < obj->propSize; ++i)
        {
            hash = hashValue(obj->keys[i]);
            slot = hash & (obj->propCapacity - 1);

            for (;;)
            {
                if (((obj->bitmap[slot / 8]) & (1 << (slot % 8))) == 0)
                {
                    obj->bitmap[slot / 8] |= (1 << (slot % 8));
                    obj->buckets[slot] = (uint32_t)i;
                    break;
                }
                slot = (slot + 1) & (obj->propCapacity - 1);
            }
        }
    }

    hash = hashValue(key);
    slot = hash & (obj->propCapacity - 1);

    for (;;)
    {
        if ((obj->bitmap[slot / 8] & (1 << (slot % 8))) == 0)
        {
            index = obj->propSize++;
            obj->keys[index] = key;
            obj->values[index] = value;
            obj->bitmap[slot / 8] |= (1 << (slot % 8));
            obj->buckets[slot] = index;
            return;
        }
        index = obj->buckets[slot];
        if (keyCompare(vm, obj->keys[index], key))
        {
            obj->values[index] = value;
            return;
        }
        slot = (slot + 1) & (obj->propCapacity - 1);
    }
}

NsValue nsVmGetObjectProperty(NsVirtualMachine* vm, NsValue object, NsValue key)
{
    NsVmObjectTable* tbl = &vm->tblObjects;
    NsObject* obj = getObject(tbl, object);
    uint64_t hash = hashValue(key);
    uint32_t slot = hash & (obj->propCapacity - 1);
    uint32_t index;

    for (;;)
    {
        if ((obj->bitmap[slot / 8] & (1 << slot % 8)) == 0)
            return 0;
        index = obj->buckets[slot];
        if (keyCompare(vm, obj->keys[index], key))
            return obj->values[index];
        slot = (slot + 1) & (obj->propCapacity - 1);
    }
}
