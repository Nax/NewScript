#include <stdio.h>
#include <libnewscript/vm/op.h>

NsValue nsNativeCall(NsVirtualMachine* vm, const void* native, uint8_t argCount, NsValue* args);

NS_OP_PROTO2(GGETI)
{
    NsValue value;

    value = nsVmGetObjectProperty(vm, vm->global, a1.value);
    vm->regsWindow[a0.u16] = value;
}

NS_OP_PROTO3(GETI)
{
    NsValue value;

    value = nsVmGetObjectProperty(vm, vm->regsWindow[a1.u16], a2.value);
    vm->regsWindow[a0.u16] = value;
}

NS_OP_PROTO2(LOADI)
{
    vm->regsWindow[a0.u16] = a1.value;
}

static void returnFrameNoExit(NsVirtualMachine* vm, NsValue value)
{
    vm->regsWindow[0] = value;
    vm->regsWindow = vm->frames[--vm->frameCounter];
}

static void returnFrame(NsVirtualMachine* vm, NsValue value)
{
    if (vm->frameCounter == 0)
        longjmp(vm->exitHandler, 1);
    returnFrameNoExit(vm, value);
}

NS_OP_PROTO3(CALL)
{
    uint16_t funcValue = a0.u16;
    uint16_t rbase = a1.u16;
    uint8_t argCount = a2.u8;
    const NsFunction* func;

    func = nsVmGetFunction(vm, vm->regsWindow[funcValue]);
    vm->frames[vm->frameCounter++] = vm->regsWindow;
    vm->regsWindow += rbase;

    if (func->flags & NS_FF_NATIVE)
    {
        returnFrameNoExit(vm, nsNativeCall(vm, func->nativeFunction, func->argCount, vm->regsWindow));
    }
}

NS_OP_PROTO1(RET)
{
    returnFrame(vm, vm->regsWindow[a0.u16]);
}

NS_OP_PROTO0(RETNIL)
{
    returnFrame(vm, 0);
}
