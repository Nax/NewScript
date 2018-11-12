#ifndef BYTECODE_OP_H
#define BYTECODE_OP_H

#include <stdint.h>

#define NS_ENC_NONE     0
#define NS_ENC_IMM8     1
#define NS_ENC_IMM16    2
#define NS_ENC_IMM32    3
#define NS_ENC_IMM64    4
#define NS_ENC_REG      5

#define NS_OP_LOADI     0x22
#define NS_OP_GGET      0x40
#define NS_OP_GET       0x41
#define NS_OP_GGETI     0x44
#define NS_OP_GETI      0x45
#define NS_OP_CALL      0x60
#define NS_OP_RET       0x70
#define NS_OP_RETNIL    0x71

typedef struct {
    const char*     name;
    uint8_t         encCount;
    uint8_t         enc[3];
} NsOp;

extern const NsOp nsOps[256];

#endif
