#ifndef BYTECODE_BUILDER_H
#define BYTECODE_BUILDER_H

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
    size_t              codeCapacity;
    char*               code;
    size_t              strTableHeaderSize;
    size_t              strTableHeaderCapacity;
    NsBytecodeStr*      strTableHeader;
    size_t              strTableSize;
    size_t              strTableCapacity;
    char*               strTable;
    size_t              relTableSize;
    size_t              relTableCapacity;
    NsBytecodeRel*      relTable;
} NsBytecodeBuilder;

NsBytecodeBuilder*  nsCreateBytecodeBuilder(void);
void                nsDestroyBytecodeBuilder(NsBytecodeBuilder* builder);

void    nsEmitBytecode8(NsBytecodeBuilder* builder, uint8_t value);
void    nsEmitBytecode16(NsBytecodeBuilder* builder, uint16_t value);
void    nsEmitBytecode32(NsBytecodeBuilder* builder, uint32_t value);
void    nsEmitBytecode64(NsBytecodeBuilder* builder, uint64_t value);
void    nsEmitBytecodeReg(NsBytecodeBuilder* builder, uint16_t value);
void    nsEmitBytecodeString(NsBytecodeBuilder* builder, const char* str, size_t length);
void    nsEmitBytecodeStringC(NsBytecodeBuilder* builder, const char* str);

uint32_t nsBytecodeInternString(NsBytecodeBuilder* builder, const char* str, size_t length);

void    nsDumpBytecode(NsBytecodeBuilder* builder);

#endif
