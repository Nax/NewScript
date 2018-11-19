.globl _kNsThreadTable
.globl _nsRunThread

.macro NEXT
lodsq
jmp *%rax
.endm



/*
 * RDI = State
 * RSI = Thread
 */
.text
.align 16
_nsRunThread:
    mov %rdi, %rbx
    NEXT

opLOADI:
    NEXT

opGGET:
    NEXT

opGGETI:
    NEXT

.data
.align 16
_kNsThreadTable:
    .quad 0, 0, 0, 0, 0, 0, 0, 0
    .quad 0, 0, 0, 0, 0, 0, 0, 0
    .quad 0, 0, 0, 0, 0, 0, 0, 0
    .quad 0, 0, 0, 0, 0, 0, 0, 0
    .quad 0, 0, opLOADI, 0, 0, 0, 0, 0
    .quad 0, 0, 0, 0, 0, 0, 0, 0
    .quad 0, 0, 0, 0, 0, 0, 0, 0
    .quad 0, 0, 0, 0, 0, 0, 0, 0
    .quad opGGET, 0, 0, 0, opGGETI, 0, 0, 0
    .quad 0, 0, 0, 0, 0, 0, 0, 0
    .quad 0, 0, 0, 0, 0, 0, 0, 0
    .quad 0, 0, 0, 0, 0, 0, 0, 0
    .quad 0, 0, 0, 0, 0, 0, 0, 0
    .quad 0, 0, 0, 0, 0, 0, 0, 0
    .quad 0, 0, 0, 0, 0, 0, 0, 0
    .quad 0, 0, 0, 0, 0, 0, 0, 0
    .quad 0, 0, 0, 0, 0, 0, 0, 0
    .quad 0, 0, 0, 0, 0, 0, 0, 0
    .quad 0, 0, 0, 0, 0, 0, 0, 0
    .quad 0, 0, 0, 0, 0, 0, 0, 0
    .quad 0, 0, 0, 0, 0, 0, 0, 0
    .quad 0, 0, 0, 0, 0, 0, 0, 0
    .quad 0, 0, 0, 0, 0, 0, 0, 0
    .quad 0, 0, 0, 0, 0, 0, 0, 0
    .quad 0, 0, 0, 0, 0, 0, 0, 0
    .quad 0, 0, 0, 0, 0, 0, 0, 0
    .quad 0, 0, 0, 0, 0, 0, 0, 0
    .quad 0, 0, 0, 0, 0, 0, 0, 0
    .quad 0, 0, 0, 0, 0, 0, 0, 0
    .quad 0, 0, 0, 0, 0, 0, 0, 0
    .quad 0, 0, 0, 0, 0, 0, 0, 0
    .quad 0, 0, 0, 0, 0, 0, 0, 0
