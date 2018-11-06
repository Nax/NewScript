#ifndef BLOB_H
#define BLOB_H

#include <stddef.h>

typedef struct 
{
    char*   data;
    size_t  size;
} NsBlob;

void nsBlobInit(NsBlob* blob);
void nsBlobInitMemory(NsBlob* blob, const char* mem, size_t size);
void nsBlobFree(NsBlob* blob);
void nsBlobCopy(NsBlob* dst, const NsBlob* src);
void nsBlobCopyMemory(NsBlob* dst, const char* mem, size_t size);
int  nsBlobEqual(const NsBlob* a, const NsBlob* b);
int  nsBlobEqualMemory(const NsBlob* blob, const char* mem, size_t size);

#endif
