#include <string.h>
#include <libnewscript/bytecode/bytecode.h>
#include <libnewscript/bytecode/op.h>
#include <libnewscript/vm/op.h>

#define _(a)    (nsOp ## a)

static const void* opFunctions[256] = {
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, _(LOADI), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, _(GGETI), _(GETI), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    _(CALL), NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
    NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
};

#undef _

typedef void (NsOpFunction0)(NsVirtualMachine*);
typedef void (NsOpFunction1)(NsVirtualMachine*, NsOpArg);
typedef void (NsOpFunction2)(NsVirtualMachine*, NsOpArg, NsOpArg);
typedef void (NsOpFunction3)(NsVirtualMachine*, NsOpArg, NsOpArg, NsOpArg);

static uint8_t readBytecode8(NsVirtualMachine* vm, NsBytecode* bc)
{
    uint8_t value;

    value = *(uint8_t*)(bc->code + vm->ip);
    vm->ip += 1;
    return value;
}

static uint16_t readBytecode16(NsVirtualMachine* vm, NsBytecode* bc)
{
    uint16_t value;

    value = *(uint16_t*)(bc->code + vm->ip);
    vm->ip += 2;
    return value;
}

static uint32_t readBytecode32(NsVirtualMachine* vm, NsBytecode* bc)
{
    uint32_t value;

    value = *(uint32_t*)(bc->code + vm->ip);
    vm->ip += 4;
    return value;
}

static uint64_t readBytecode64(NsVirtualMachine* vm, NsBytecode* bc)
{
    uint64_t value;

    value = *(uint64_t*)(bc->code + vm->ip);
    vm->ip += 8;
    return value;
}

static uint16_t readBytecodeReg(NsVirtualMachine* vm, NsBytecode* bc)
{
    uint16_t value;

    value = *(uint8_t*)(bc->code + vm->ip);
    vm->ip += 1;
    if (value & 0x80)
    {
        value = (value & 0x7f) | (*(uint8_t*)(bc->code + vm->ip) << 7);
        vm->ip += 1;
    }
    return value;
}

static NsOpArg readBytecodeArg(NsVirtualMachine* vm, NsBytecode* bc, uint8_t enc)
{
    NsOpArg arg;

    memset(&arg, 0, sizeof(arg));
    switch (enc)
    {
    case NS_ENC_IMM8:
        arg.u8 = readBytecode8(vm, bc);
        break;
    case NS_ENC_IMM16:
        arg.u16 = readBytecode16(vm, bc);
        break;
    case NS_ENC_IMM32:
        arg.u32 = readBytecode32(vm, bc);
        break;
    case NS_ENC_IMM64:
        arg.u64 = readBytecode64(vm, bc);
        break;
    case NS_ENC_REG:
        arg.u16 = readBytecodeReg(vm, bc);
        break;
    }

    return arg;
}

void nsExecBytecode(NsVirtualMachine* vm, NsBytecode* bc)
{
    uint8_t op;
    NsOpArg args[3];

    for (;;)
    {
        op = readBytecode8(vm, bc);
        switch (nsOps[op].encCount)
        {
        case 0:
            ((NsOpFunction0*)opFunctions[op])(vm);
            break;
        case 1:
            args[0] = readBytecodeArg(vm, bc, nsOps[op].enc[0]);
            ((NsOpFunction1*)opFunctions[op])(vm, args[0]);
            break;
        case 2:
            args[0] = readBytecodeArg(vm, bc, nsOps[op].enc[0]);
            args[1] = readBytecodeArg(vm, bc, nsOps[op].enc[1]);
            ((NsOpFunction2*)opFunctions[op])(vm, args[0], args[1]);
            break;
        case 3:
            args[0] = readBytecodeArg(vm, bc, nsOps[op].enc[0]);
            args[1] = readBytecodeArg(vm, bc, nsOps[op].enc[1]);
            args[2] = readBytecodeArg(vm, bc, nsOps[op].enc[2]);
            ((NsOpFunction3*)opFunctions[op])(vm, args[0], args[1], args[2]);
            break;
        }
    }
}
