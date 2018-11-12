#ifndef VM_VM_H
#define VM_VM_H

#include <libnewscript/vm/function.h>
#include <libnewscript/vm/object.h>
#include <libnewscript/vm/string.h>

typedef struct NsBytecode_ NsBytecode;
typedef struct NsVirtualMachine_ NsVirtualMachine;

struct NsVirtualMachine_ {
    NsVmFunctionTable   tblFunctions;
    NsVmObjectTable     tblObjects;
    NsVmStringTable     tblStrings;
    NsValue             global;
    NsValue*            regs;
    NsValue*            regsWindow;
    NsValue*            frames[1024];
    size_t              frameCounter;
    size_t              ip;
};

NsVirtualMachine*   nsCreateVirtualMachine(void);
void                nsDestroyVirtualMachine(NsVirtualMachine* vm);
void                nsVmLinkBytecode(NsVirtualMachine* vm, NsBytecode* bc);
void                nsExecBytecode(NsVirtualMachine* vm, NsBytecode* bc);

#endif
