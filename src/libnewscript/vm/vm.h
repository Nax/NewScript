#ifndef VM_VM_H
#define VM_VM_H

typedef struct {

} NsVirtualMachine;

NsVirtualMachine*   nsCreateVirtualMachine(void);
void                nsDestroyVirtualMachine(NsVirtualMachine* vm);



#endif
