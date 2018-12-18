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

%define STACK_SIZE  0x58

; RDI = State
; RSI = Thread
; R14 = Global
; R15 = Register File
_nsRunThread:
  sub rsp, STACK_SIZE
  mov [rsp + (STACK_SIZE - 0x30)], rbx
  mov [rsp + (STACK_SIZE - 0x28)], rbp
  mov [rsp + (STACK_SIZE - 0x20)], r12
  mov [rsp + (STACK_SIZE - 0x18)], r13
  mov [rsp + (STACK_SIZE - 0x10)], r14
  mov [rsp + (STACK_SIZE - 0x08)], r15

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

  push rdi
  push rsi

  mov rsi, [r15 + 8 * r12]
  call _nsVmGetFunction
  mov r12, [rax]

  pop rsi
  pop rdi

  mov  rax, rsi
  push rsi
  push rdi

  cmp rbp, 5
  ja call6p
  lea r11, [REL CallJmpTable]
  jmp [r11 + rbp * 8]

call6p:
  jmp callNative

call5:
  movzx r9, WORD [rax + 0xe]
  mov r9, QWORD [r15 + 8 * r9]
call4:
  movzx r8, WORD [rax + 0xc]
  mov r8, QWORD [r15 + 8 * r8]
call3:
  movzx rcx, WORD [rax + 0xa]
  mov rcx, QWORD [r15 + 8 * rcx]
call2:
  movzx rdx, WORD [rax + 0x8]
  mov rdx, QWORD [r15 + 8 * rdx]
call1:
  movzx rsi, WORD [rax + 0x6]
  mov rsi, QWORD [r15 + 8 * rsi]

callNative:
  call r12

  pop rdi
  pop rsi

  ; Deal with thread alignment
  lea rbp, [rbp * 2 + 5]
  add rsi, rbp
  or rsi, 0x7
  add rsi, 1

  NEXT

opRET:
  NEXT

opRETNIL:
  jmp yieldVM
  NEXT

unwind:


yieldVM:
  mov rbx, [rsp + (STACK_SIZE - 0x30)]
  mov rbp, [rsp + (STACK_SIZE - 0x28)]
  mov r12, [rsp + (STACK_SIZE - 0x20)]
  mov r13, [rsp + (STACK_SIZE - 0x18)]
  mov r14, [rsp + (STACK_SIZE - 0x10)]
  mov r15, [rsp + (STACK_SIZE - 0x08)]
  add rsp, STACK_SIZE
  ret

SECTION .rodata
ALIGN 16
CallJmpTable:
  dq callNative
  dq call1
  dq call2
  dq call3
  dq call4
  dq call5

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
