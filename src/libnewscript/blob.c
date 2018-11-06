#include <stdlib.h>
#include <string.h>
#include <libnewscript/blob.h>

void nsBlobInit(NsBlob* blob)
{
    blob->data = malloc(1);
    blob->data[0] = 0;
    blob->size = 0;
}

void nsBlobInitMemory(NsBlob* blob, const char* mem, size_t size)
{
    blob->data = malloc(size + 1);
    memcpy(blob->data, mem, size);
    blob->data[size] = 0;
    blob->size = size;
}

void nsBlobFree(NsBlob* blob)
{
    free(blob->data);
}

void nsBlobCopy(NsBlob* dst, const NsBlob* src)
{
    nsBlobCopyMemory(dst, src->data, src->size);
}

void nsBlobCopyMemory(NsBlob* dst, const char* mem, size_t size)
{
    nsBlobFree(dst);
    nsBlobInitMemory(dst, mem, size);
}

int nsBlobEqual(const NsBlob* a, const NsBlob* b)
{
    return nsBlobEqualMemory(a, b->data, b->size);
}

int nsBlobEqualMemory(const NsBlob* blob, const char* mem, size_t size)
{
    if (size != blob->size)
        return 0;

    return (memcmp(blob->data, mem, size) == 0);
}
