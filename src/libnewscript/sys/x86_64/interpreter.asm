SECTION .text

EXTERN _nsVmGetObjectProperty
EXTERN _nsVmGetFunction

GLOBAL _kNsThreadTable
GLOBAL _nsRunThread

ALIGN 16

%macro NEXT 0
lodsq
jmp rax
%endmacro

%define STACK_SIZE  0x40

; RDI = State
; RSI = Thread
; R14 = Global
; R15 = Register File
_nsRunThread:
  sub rsp, STACK_SIZE
  mov [rsp + (STACK_SIZE - 0x28)], rbx
  mov [rsp + (STACK_SIZE - 0x20)], rbp
  mov [rsp + (STACK_SIZE - 0x18)], r12
  mov [rsp + (STACK_SIZE - 0x10)], r13
  mov [rsp + (STACK_SIZE - 0x08)], r14
  mov [rsp + (STACK_SIZE - 0x00)], r15

  mov r14, [rdi + 0 * 8]
  mov r15, [rdi + 1 * 8]
  mov rbx, rdi
  NEXT

opLOADI:
  movzx rcx, WORD [rsi]
  mov rdx, [rsi + 8]
  add rsi, 16

  mov [r15 + rcx * 8], rdx
  NEXT

opGGETI:
  movzx r12, WORD [rsi]
  mov rdx, [rsi + 0x08]
  add rsi, 16
  mov [rsp + 0x08], rsi
  mov rdi, rbx
  mov rsi, r14
  call _nsVmGetObjectProperty
  mov rsi, [rsp + 0x08]
  mov [r15 + 8 * r12], rax
  NEXT

opGETI:
  movzx r12, WORD [rsi]
  movzx rcx, WORD [rsi + 2]
  mov rcx, [r15 + 8 * rcx]
  mov rdx, [rsi + 0x08]
  add rsi, 16
  mov [rsp + 0x08], rsi
  mov rdi, rbx
  mov rsi, rcx
  call _nsVmGetObjectProperty
  mov rsi, [rsp + 0x08]
  mov [r15 + 8 * r12], rax
  NEXT

opCALL:
  movzx r12, WORD [rsi]
  movzx r13, WORD [rsi + 2]
  movzx rbp, BYTE [rsi + 4]

  NEXT

opRET:
  NEXT

opRETNIL:
  NEXT

unwind:


yieldVM:
  mov rbx, [rsp + (STACK_SIZE - 0x28)]
  mov rbp, [rsp + (STACK_SIZE - 0x20)]
  mov r12, [rsp + (STACK_SIZE - 0x18)]
  mov r13, [rsp + (STACK_SIZE - 0x10)]
  mov r14, [rsp + (STACK_SIZE - 0x08)]
  mov r15, [rsp + (STACK_SIZE - 0x00)]
  add rsp, STACK_SIZE
  ret

SECTION .rodata
ALIGN 16
_kNsThreadTable:
  dq 0, 0, 0, 0, 0, 0, 0, 0
  dq 0, 0, 0, 0, 0, 0, 0, 0
  dq 0, 0, 0, 0, 0, 0, 0, 0
  dq 0, 0, 0, 0, 0, 0, 0, 0
  dq 0, 0, opLOADI, 0, 0, 0, 0, 0
  dq 0, 0, 0, 0, 0, 0, 0, 0
  dq 0, 0, 0, 0, 0, 0, 0, 0
  dq 0, 0, 0, 0, 0, 0, 0, 0
  dq 0, 0, 0, 0, opGGETI, opGETI, 0, 0
  dq 0, 0, 0, 0, 0, 0, 0, 0
  dq 0, 0, 0, 0, 0, 0, 0, 0
  dq 0, 0, 0, 0, 0, 0, 0, 0
  dq opCALL, 0, 0, 0, 0, 0, 0, 0
  dq 0, 0, 0, 0, 0, 0, 0, 0
  dq opRET, opRETNIL, 0, 0, 0, 0, 0, 0
  dq 0, 0, 0, 0, 0, 0, 0, 0
  dq 0, 0, 0, 0, 0, 0, 0, 0
  dq 0, 0, 0, 0, 0, 0, 0, 0
  dq 0, 0, 0, 0, 0, 0, 0, 0
  dq 0, 0, 0, 0, 0, 0, 0, 0
  dq 0, 0, 0, 0, 0, 0, 0, 0
  dq 0, 0, 0, 0, 0, 0, 0, 0
  dq 0, 0, 0, 0, 0, 0, 0, 0
  dq 0, 0, 0, 0, 0, 0, 0, 0
  dq 0, 0, 0, 0, 0, 0, 0, 0
  dq 0, 0, 0, 0, 0, 0, 0, 0
  dq 0, 0, 0, 0, 0, 0, 0, 0
  dq 0, 0, 0, 0, 0, 0, 0, 0
  dq 0, 0, 0, 0, 0, 0, 0, 0
  dq 0, 0, 0, 0, 0, 0, 0, 0
  dq 0, 0, 0, 0, 0, 0, 0, 0
  dq 0, 0, 0, 0, 0, 0, 0, 0
