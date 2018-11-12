#include <stdlib.h>
#include <libnewscript/vm/value.h>
#include <libnewscript/vm/string.h>
#include <libnewscript/vm/vm.h>

NsVirtualMachine* nsCreateVirtualMachine(void)
{
    NsVirtualMachine* vm;

    vm = malloc(sizeof(*vm));
    nsVmInitStringTable(&vm->tblStrings);
    return vm;
}

void nsDestroyVirtualMachine(NsVirtualMachine* vm)
{
    nsVmDeinitStringTable(&vm->tblStrings);
    free(vm);
}
