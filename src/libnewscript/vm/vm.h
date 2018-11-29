#ifndef VM_VM_H
#define VM_VM_H

#include <setjmp.h>
#include <libnewscript/vm/function.h>
#include <libnewscript/vm/object.h>
#include <libnewscript/vm/string.h>

typedef struct NsBytecode_ NsBytecode;
typedef struct NsVirtualMachine_ NsVirtualMachine;

typedef struct {
    NsValue*    ptr;
    size_t      regCount;
} NsVmFrame;

struct NsVirtualMachine_ {
    NsValue             global;
    NsValue*            regs;
    NsValue*            regsWindow;
    NsVmFunctionTable   tblFunctions;
    NsVmObjectTable     tblObjects;
    NsVmStringTable     tblStrings;
    NsVmFrame           frames[1024];
    size_t              frameCounter;
    size_t              ip;
    size_t              regCount;
    jmp_buf             exitHandler;
};

NsVirtualMachine*   nsCreateVirtualMachine(void);
void                nsDestroyVirtualMachine(NsVirtualMachine* vm);
void                nsVmLinkBytecode(NsVirtualMachine* vm, NsBytecode* bc);
void                nsExecBytecode(NsVirtualMachine* vm, NsBytecode* bc);

#endif
