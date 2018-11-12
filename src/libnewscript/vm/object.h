#ifndef OBJECT_H
#define OBJECT_H

#include <stdint.h>
#include <stddef.h>
#include <libnewscript/vm/value.h>

typedef struct _NsVirtualMachine NsVirtualMachine;

typedef struct {
    size_t      propSize;
    size_t      propCapacity;
    NsValue*    keys;
    NsValue*    values;
    uint32_t*   buckets;
    uint8_t*    bitmap;
} NsObject;

typedef struct {
    size_t      size;
    size_t      capacity;
    NsObject*   objects;
} NsVmObjectTable;

void nsVmInitObjectTable(NsVmObjectTable* tbl);
void nsVmDeinitObjectTable(NsVmObjectTable* tbl);

NsValue     nsVmCreateObject(NsVirtualMachine* vm);
void        nsVmSetObjectProperty(NsVirtualMachine* vm, NsValue object, NsValue key, NsValue value);
NsValue     nsVmGetObjectProperty(NsVirtualMachine* vm, NsValue object, NsValue key);

#endif
