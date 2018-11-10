#include <stdlib.h>
#include <string.h>
#include <libnewscript/hash.h>

static uint64_t nsHashString(const char* str)
{
    uint64_t hash = 0xcbf29ce484222325;
    uint8_t d;
    size_t i = 0;

    for (;;)
    {
        d = (uint8_t)str[i++];
        if (!d)
            break;
        hash = ((hash ^ d) * 0x100000001b3);
    }
    return hash;
}

NsStringMap32* nsCreateStringMap32(void)
{
    NsStringMap32* strMap;

    strMap = malloc(sizeof(*strMap));
    strMap->count = 0;
    strMap->bucketCapacity = 16;
    strMap->dataCapacity = 4;
    strMap->bitmap = calloc(1, strMap->bucketCapacity / 8);
    strMap->buckets = malloc(strMap->bucketCapacity * sizeof(uint32_t));
    strMap->keys = malloc(sizeof(char*) * strMap->dataCapacity);
    strMap->values = malloc(sizeof(uint32_t) * strMap->dataCapacity);

    return strMap;
}

void nsDestroyStringMap32(NsStringMap32* strMap)
{
    for (size_t i = 0; i < strMap->count; ++i)
        free(strMap->keys[i]);
    free(strMap->buckets);
    free(strMap->bitmap);
    free(strMap->keys);
    free(strMap->values);
    free(strMap);
}

uint32_t nsGetStringMap32(const NsStringMap32* strMap, const char* key)
{
    uint64_t hash = nsHashString(key);
    size_t slot = hash & (strMap->bucketCapacity - 1);

    for (;;)
    {
        if ((strMap->bitmap[slot / 8] & (1 << (slot % 8))) == 0)
            return 0;

        uint32_t index = strMap->buckets[slot];
        if (strcmp(key, strMap->keys[index]) == 0)
            return strMap->values[index];

        slot = (slot + 1) & (strMap->bucketCapacity - 1);
    }
}

void nsSetStringMap32(NsStringMap32* strMap, const char* key, uint32_t value)
{
    uint64_t hash;
    size_t slot;
    uint32_t index;

    if (strMap->count == strMap->bucketCapacity / 2)
    {
        size_t newCapacity = strMap->bucketCapacity * 2;
        free(strMap->bitmap);
        free(strMap->buckets);
        strMap->bitmap = calloc(1, newCapacity / 8);
        strMap->buckets = malloc(newCapacity * sizeof(uint32_t));
        strMap->bucketCapacity = newCapacity;

        for (index = 0; index < strMap->count; ++index)
        {
            hash = nsHashString(strMap->keys[index]);
            slot = hash & (strMap->bucketCapacity - 1);

            for (;;)
            {
                if ((strMap->bitmap[slot / 8] & (1 << (slot % 8))) == 0)
                {
                    strMap->bitmap[slot / 8] |= (1 << (slot % 8));
                    strMap->buckets[slot] = index;
                    break;
                }
                slot = (slot + 1) & (strMap->bucketCapacity - 1);
            }
        }
    }

    hash = nsHashString(key);
    slot = hash & (strMap->bucketCapacity - 1);

    for (;;)
    {
        if ((strMap->bitmap[slot / 8] & (1 << (slot % 8))) == 0)
        {
            /* New Key */
            strMap->bitmap[slot / 8] |= (1 << (slot % 8));
            if (strMap->dataCapacity == strMap->count)
            {
                size_t newCapacity = strMap->dataCapacity + strMap->dataCapacity / 2;
                strMap->keys = realloc(strMap->keys, sizeof(char*) * newCapacity);
                strMap->values = realloc(strMap->values, sizeof(uint32_t) * newCapacity);
                strMap->dataCapacity = newCapacity;
            }
            index = strMap->count++;
            strMap->keys[index] = strdup(key);
            strMap->values[index] = value;
            strMap->buckets[slot] = index;
            return;
        }

        index = strMap->buckets[slot];
        if (strcmp(key, strMap->keys[index]) == 0)
        {
            /* Replace */
            strMap->values[index] = value;
            return;
        }

        slot = (slot + 1) & (strMap->bucketCapacity - 1);
    }
}
