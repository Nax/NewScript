SECTION .text
GLOBAL _kNsThreadTable
GLOBAL _nsRunThread
ALIGN 16

%macro NEXT 0
lodsq
jmp rax
%endmacro

; RDI = State
; RSI = Thread
; R15 = Register File
_nsRunThread:
  mov r15, [rdi + 1 * 8]
  mov rbx, rdi
  NEXT

opLOADI:
  movzx cx, [rsi]
  mov rdx, [rsi + 8]
  add rsi, 16

  mov [r15 + rcx * 8], rdx
  NEXT

opGGET:
  NEXT

opGGETI:
  NEXT



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
  dq opGGET, 0, 0, 0, opGGETI, 0, 0, 0
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
  dq 0, 0, 0, 0, 0, 0, 0, 0
  dq 0, 0, 0, 0, 0, 0, 0, 0
  dq 0, 0, 0, 0, 0, 0, 0, 0
  dq 0, 0, 0, 0, 0, 0, 0, 0
  dq 0, 0, 0, 0, 0, 0, 0, 0
  dq 0, 0, 0, 0, 0, 0, 0, 0
