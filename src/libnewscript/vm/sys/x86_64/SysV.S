.globl _nsNativeCall

/*
 * rdi: VM
 * rsi: Native Ptr
 * rdx: Arg Count
 * rcx: Args
 */
_nsNativeCall:
    movq %rsi, %rax
    movq %rdx, %r10
    movq %rcx, %r11

    movq 0x00(%r11), %rsi
    movq 0x08(%r11), %rdx
    movq 0x10(%r11), %rcx
    movq 0x18(%r11), %r8
    movq 0x20(%r11), %r9

    jmp *%rax
