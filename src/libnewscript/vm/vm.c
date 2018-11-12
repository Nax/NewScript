#include <stdlib.h>
#include <stdio.h>
#include <libnewscript/vm/value.h>
#include <libnewscript/vm/object.h>
#include <libnewscript/vm/string.h>
#include <libnewscript/vm/vm.h>

NsVirtualMachine* nsCreateVirtualMachine(void)
{
    NsVirtualMachine* vm;

    vm = malloc(sizeof(*vm));
    nsVmInitObjectTable(&vm->tblObjects);
    nsVmInitStringTable(&vm->tblStrings);

    vm->global = nsVmCreateObject(vm);
    nsVmSetObjectProperty(vm, vm->global, nsVmCreateCString(vm, "Global", 1), vm->global);
    printf("Global:    0x%016llx\n", vm->global);
    printf("Global2:   0x%016llx\n", nsVmGetObjectProperty(vm, vm->global, nsVmCreateCString(vm, "Global", 1)));
    return vm;
}

void nsDestroyVirtualMachine(NsVirtualMachine* vm)
{
    nsVmDeinitObjectTable(&vm->tblObjects);
    nsVmDeinitStringTable(&vm->tblStrings);
    free(vm);
}
