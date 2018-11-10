# NewScript Bytecode

## Packed Integer

Whether the integer is signed or not is irrelevant, as the packing only concern itself with bits.

### RegPack

+---+---------+
| 0 | xxxxxxx |
+---+---------+

or

+---+---------+----------+
| 1 | xxxxxxx | xxxxxxxx |
+---+---------+----------+

Up to 15 bits of effective storage (32768 registers).

## Encoding

### Parameters

x:      No parameter
imm8:   1 byte, raw data
imm16:  2 bytes, raw data
imm32:  4 bytes, raw data
imm64:  8 bytes, raw data
r:      1-2 bytes, reg packed

## Opcodes

### Summary

|   |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7   |   8   |   9   |   a   |   b   |   c   |   d   |   e   |   f   |
|---|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|-------|
| 0 |  NOT  |  AND  |  XOR  |  OR   |  NEG  | ANDI  | XORI  |  ORI  |       |       |       |       |       |       |       |       |
| 1 |       |       |       |       |       |       |       |       |       |       |       |       |       |       |       |       |
| 2 |  MOV  | LOAD  |       |       |       |       |       |       |       |       |       |       |       |       |       |       |
| 3 |       |       |       |       |       |       |       |       |       |       |       |       |       |       |       |       |
| 4 |  GET  | SGET  |       |       |       |       |       |       |       |       |       |       |       |       |       |       |
| 5 |       |       |       |       |       |       |       |       |       |       |       |       |       |       |       |       |
| 6 |       |       |       |       |       |       |       |       |       |       |       |       |       |       |       |       |
| 7 |       |       |       |       |       |       |       |       |       |       |       |       |       |       |       |       |
| 8 |       |       |       |       |       |       |       |       |       |       |       |       |       |       |       |       |
| 9 |       |       |       |       |       |       |       |       |       |       |       |       |       |       |       |       |
| a |       |       |       |       |       |       |       |       |       |       |       |       |       |       |       |       |
| b |       |       |       |       |       |       |       |       |       |       |       |       |       |       |       |       |
| c |       |       |       |       |       |       |       |       |       |       |       |       |       |       |       |       |
| d |       |       |       |       |       |       |       |       |       |       |       |       |       |       |       |       |
| e |       |       |       |       |       |       |       |       |       |       |       |       |       |       |       |       |
| f |       |       |       |       |       |       |       |       |       |       |       |       |       |       |       |       |

### 0x00 NOT

### 0x20 MOV

### 0x21 LOAD reg, imm64



