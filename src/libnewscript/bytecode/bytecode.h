#ifndef BYTECODE_H
#define BYTECODE_H

#include <stddef.h>
#include <stdint.h>

#define NS_REL_NONE     0
#define NS_REL_STRING   1

typedef struct {
    uint32_t    offset;
    uint32_t    length;
} NsBytecodeStr;

typedef struct {
    uint32_t    offset;
    uint32_t    index;
    uint8_t     type;
} NsBytecodeRel;

typedef struct {
    size_t              codeSize;
    char*               code;
    size_t              strTableHeaderSize;
    NsBytecodeStr*      strTableHeader;
    size_t              strTableSize;
    char*               strTable;
    size_t              relTableSize;
    NsBytecodeRel*      relTable;
} NsBytecode;

void    nsDestroyBytecode(NsBytecode* bytecode);
void    nsDumpBytecode(NsBytecode* bytecode);

#endif
