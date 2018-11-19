#ifndef THREAD_H
#define THREAD_H

#include <libnewscript/vm/vm.h>
#include <libnewscript/bytecode/bytecode.h>

typedef struct {
    size_t  size;
    size_t  capacity;
    void*   data;
} NsThreadBuilder;

void*   nsThreadBuild(NsVirtualMachine* vm, NsBytecode* bc);
void    nsRunThread(NsVirtualMachine* vm, void* thread);

#endif
