#ifndef VM_FUNCTION_H
#define VM_FUNCTION_H

#include <stdint.h>
#include <stddef.h>
#include <libnewscript/vm/value.h>

typedef struct NsVirtualMachine_ NsVirtualMachine;

#define NS_FF_NATIVE    0x01
#define NS_FF_JITTED    0x02

typedef struct {
    const void*     nativeFunction;
    uint8_t         flags;
    uint8_t         argCount;
} NsFunction;

typedef struct {
    size_t          size;
    size_t          capacity;
    NsFunction*     functions;
} NsVmFunctionTable;

void nsVmInitFunctionTable(NsVmFunctionTable* tbl);
void nsVmDeinitFunctionTable(NsVmFunctionTable* tbl);

NsValue             nsVmCreateFunctionNative(NsVirtualMachine* vm, uint8_t argCount, const void* funcPtr);
const NsFunction*   nsVmGetFunction(NsVirtualMachine* vm, NsValue value);

#endif
