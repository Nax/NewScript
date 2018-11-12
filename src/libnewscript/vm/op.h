#ifndef VM_OP_H
#define VM_OP_H

#include <libnewscript/vm/vm.h>

typedef union {
    uint8_t     u8;
    uint16_t    u16;
    uint32_t    u32;
    uint64_t    u64;
    NsValue     value;
} NsOpArg;

#define NS_OP_PROTO0(name)  void nsOp ## name (NsVirtualMachine* vm)
#define NS_OP_PROTO1(name)  void nsOp ## name (NsVirtualMachine* vm, NsOpArg a0)
#define NS_OP_PROTO2(name)  void nsOp ## name (NsVirtualMachine* vm, NsOpArg a0, NsOpArg a1)
#define NS_OP_PROTO3(name)  void nsOp ## name (NsVirtualMachine* vm, NsOpArg a0, NsOpArg a1, NsOpArg a2)

NS_OP_PROTO2(GGETI);
NS_OP_PROTO3(GETI);
NS_OP_PROTO2(LOADI);
NS_OP_PROTO3(CALL);

#endif
