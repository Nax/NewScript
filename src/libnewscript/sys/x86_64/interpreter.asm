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
_nsRunThread:
  mov rbx, rdi
  NEXT

opLOADI:
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
