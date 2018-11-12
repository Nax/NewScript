#ifndef STRING_H
#define STRING_H

#include <stdint.h>
#include <stddef.h>
#include <libnewscript/vm/value.h>

/*
 * TODO: Find a correct prefix for libnewscript.
 * Having STRING_H include guards is a receipe for disaster.
 */

typedef struct NsVirtualMachine_ NsVirtualMachine;

typedef struct {
    size_t      size;
    size_t      capacity;
    char**      data;
    size_t*     length;
    uint8_t*    interned;

    size_t      internedMapSize;
    size_t      internedMapCapacity;
    NsStringId* internedMapBuckets;
    NsStringId* internedMapList;
} NsVmStringTable;

void nsVmInitStringTable(NsVmStringTable* tbl);
void nsVmDeinitStringTable(NsVmStringTable* tbl);

NsValue nsVmCreateString(NsVirtualMachine* vm, const char* data, size_t length, int intern);
NsValue nsVmCreateCString(NsVirtualMachine* vm, const char* data, int intern);

const char* nsVmStringData(NsVirtualMachine* vm, NsValue str);

#endif
