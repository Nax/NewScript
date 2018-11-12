#ifndef VM_VM_H
#define VM_VM_H

#include <libnewscript/vm/string.h>

typedef struct _NsVirtualMachine NsVirtualMachine;

struct _NsVirtualMachine {
    NsVmStringTable     tblStrings;
};

NsVirtualMachine*   nsCreateVirtualMachine(void);
void                nsDestroyVirtualMachine(NsVirtualMachine* vm);

#endif
