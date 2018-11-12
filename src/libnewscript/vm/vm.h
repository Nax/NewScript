#ifndef VM_VM_H
#define VM_VM_H

#include <libnewscript/vm/object.h>
#include <libnewscript/vm/string.h>

typedef struct _NsVirtualMachine NsVirtualMachine;

struct _NsVirtualMachine {
    NsVmObjectTable     tblObjects;
    NsVmStringTable     tblStrings;
    NsValue             global;
};

NsVirtualMachine*   nsCreateVirtualMachine(void);
void                nsDestroyVirtualMachine(NsVirtualMachine* vm);

#endif
