#ifndef BYTECODE_BUILDER_H
#define BYTECODE_BUILDER_H

#include <stddef.h>
#include <stdint.h>
#include <libnewscript/hash.h>

typedef struct {
    uint32_t    offset;
    uint32_t    strIndex;
} NsBytecodeRelSel;

typedef struct {
    size_t              codeSize;
    size_t              codeCapacity;
    char*               code;
    size_t              strTableSize;
    size_t              strTableCapacity;
    char*               strTable;
    NsStringMap32*      strTableMap;
    size_t              relTableSize;
    size_t              relTableCapacity;
    NsBytecodeRelSel*   relTable;
} NsBytecodeBuilder;

NsBytecodeBuilder*  nsCreateBytecodeBuilder(void);
void                nsDestroyBytecodeBuilder(NsBytecodeBuilder* builder);

void    nsEmitBytecode8(NsBytecodeBuilder* builder, uint8_t value);
void    nsEmitBytecode16(NsBytecodeBuilder* builder, uint16_t value);
void    nsEmitBytecode32(NsBytecodeBuilder* builder, uint32_t value);
void    nsEmitBytecode64(NsBytecodeBuilder* builder, uint64_t value);
void    nsEmitBytecodeReg(NsBytecodeBuilder* builder, uint16_t value);
void    nsEmitBytecodeRel32(NsBytecodeBuilder* builder, const char* str);

void    nsDumpBytecode(NsBytecodeBuilder* builder);

#endif
