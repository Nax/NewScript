#include <stdlib.h>
#include <stdio.h>
#include <libnewscript/vm/vm.h>

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
    free(vm);
}
