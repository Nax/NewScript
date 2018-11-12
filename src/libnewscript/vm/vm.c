#include <stdlib.h>
#include <stdio.h>
#include <libnewscript/vm/vm.h>
#include <libnewscript/bytecode/bytecode.h>

static NsValue consoleLog(NsVirtualMachine* vm, NsValue str)
{
    const char* strData = nsVmStringData(vm, str);
    puts(strData);
    return 0;
}

NsVirtualMachine* nsCreateVirtualMachine(void)
{
    NsVirtualMachine* vm;
    NsValue console;

    vm = malloc(sizeof(*vm));
    nsVmInitFunctionTable(&vm->tblFunctions);
    nsVmInitObjectTable(&vm->tblObjects);
    nsVmInitStringTable(&vm->tblStrings);
    vm->regs = calloc(1000000, sizeof(*vm->regs));
    vm->regsWindow = vm->regs;
    vm->ip = 0;
    vm->frameCounter = 0;

    vm->global = nsVmCreateObject(vm);
    nsVmSetObjectProperty(vm, vm->global, nsVmCreateCString(vm, "Global", 1), vm->global);
    console = nsVmCreateObject(vm);
    nsVmSetObjectProperty(vm, console, nsVmCreateCString(vm, "log", 1), nsVmCreateFunctionNative(vm, 1, &consoleLog));
    nsVmSetObjectProperty(vm, vm->global, nsVmCreateCString(vm, "Console", 1), console);
    return vm;
}

void nsDestroyVirtualMachine(NsVirtualMachine* vm)
{
    nsVmDeinitFunctionTable(&vm->tblFunctions);
    nsVmDeinitObjectTable(&vm->tblObjects);
    nsVmDeinitStringTable(&vm->tblStrings);
    free(vm->regs);
    free(vm);
}

void nsVmLinkBytecode(NsVirtualMachine* vm, NsBytecode* bc)
{
    NsValue* strConstants;

    strConstants = malloc(bc->strTableHeaderSize * sizeof(*strConstants));
    for (size_t i = 0; i < bc->strTableHeaderSize; ++i)
        strConstants[i] = nsVmCreateString(vm, bc->strTable + bc->strTableHeader[i].offset, bc->strTableHeader[i].length, 1);

    for (size_t i = 0; i < bc->relTableSize; ++i)
        *(uint64_t*)(bc->code + bc->relTable[i].offset) = strConstants[bc->relTable[i].index];

    bc->strTableHeaderSize = 0;
    bc->strTableSize = 0;
    bc->relTableSize = 0;

    free(bc->strTableHeader);
    free(bc->strTable);
    free(bc->relTable);

    bc->strTableHeader = NULL;
    bc->strTable = NULL;
    bc->relTable = NULL;

    free(strConstants);
}
