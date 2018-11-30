#include <stdlib.h>
#include <libnewscript/vm/vm.h>
#include <libnewscript/vm/function.h>

void nsVmInitFunctionTable(NsVmFunctionTable* tbl)
{
    tbl->size = 1;
    tbl->capacity = 16;
    tbl->functions = malloc(tbl->capacity * sizeof(*tbl->functions));
}

void nsVmDeinitFunctionTable(NsVmFunctionTable* tbl)
{
    free(tbl->functions);
}

NsFunctionId allocFunction(NsVmFunctionTable* tbl)
{
    NsFunctionId id;
    size_t newCapacity;

    if (tbl->size == tbl->capacity)
    {
        newCapacity = tbl->capacity + tbl->capacity / 2;
        tbl->functions = realloc(tbl->functions, newCapacity * sizeof(*tbl->functions));
        tbl->capacity = newCapacity;
    }
    id = tbl->size++;
    return id;
}

NsValue nsVmCreateFunctionNative(NsVirtualMachine* vm, uint8_t argCount, const void* funcPtr)
{
    NsVmFunctionTable* tbl = &vm->tblFunctions;
    NsFunctionId funcId = allocFunction(tbl);
    NsFunction* func = tbl->functions + funcId;

    func->nativeFunction = funcPtr;
    func->flags = NS_FF_NATIVE;
    func->argCount = argCount;

    return nsMakeFunction(funcId);
}

const NsFunction* nsVmGetFunction(NsVirtualMachine* vm, NsValue value)
{
    NsVmFunctionTable* tbl = &vm->tblFunctions;
    NsFunctionId funcId = nsValueFunction(value);
    const NsFunction* func = tbl->functions + funcId;

    return func;
}
