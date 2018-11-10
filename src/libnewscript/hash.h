#ifndef HASH_H
#define HASH_H

#include <stddef.h>
#include <stdint.h>

typedef struct {
    size_t      count;
    size_t      bucketCapacity;
    size_t      dataCapacity;
    uint32_t*   buckets;
    uint8_t*    bitmap;
    char**      keys;
    uint32_t*   values;
} NsStringMap32;

NsStringMap32*  nsCreateStringMap32(void);
void            nsDestroyStringMap32(NsStringMap32* strMap);
uint32_t        nsGetStringMap32(const NsStringMap32* strMap, const char* key);
void            nsSetStringMap32(NsStringMap32* strMap, const char* key, uint32_t value);

#endif
