SECTION .text
GLOBAL _nsNativeCall
ALIGN 16

; rdi: VM
; rsi: Native Ptr
; rdx: Arg Count
; rcx: Args
_nsNativeCall:
  mov rax, rsi
  mov r10, rdx
  mov r11, rcx

  mov QWORD rsi, [r11 + 0 * 8]
  mov QWORD rdx, [r11 + 1 * 8]
  mov QWORD rcx, [r11 + 2 * 8]
  mov QWORD r8,  [r11 + 3 * 8]
  mov QWORD r9,  [r11 + 4 * 8]

  jmp [rax]
