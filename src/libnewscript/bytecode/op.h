#ifndef BYTECODE_OP_H
#define BYTECODE_OP_H

#include <stdint.h>

#define NS_ENC_NONE     0
#define NS_ENC_IMM8     1
#define NS_ENC_IMM16    2
#define NS_ENC_IMM32    3
#define NS_ENC_IMM64    4
#define NS_ENC_REG      5

#define NS_OP_GET   0x40
#define NS_OP_SGET  0x41

typedef struct {
    const char*     name;
    uint8_t         enc[3];
} NsOp;

extern const NsOp nsOps[256];

#endif
