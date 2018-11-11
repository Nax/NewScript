#ifndef BYTECODE_BUILDER_H
#define BYTECODE_BUILDER_H

#include <stddef.h>
#include <stdint.h>
#include <libnewscript/bytecode/bytecode.h>

typedef struct {
    NsBytecode* bc;
    size_t      codeCapacity;
    size_t      strTableHeaderCapacity;
    size_t      strTableCapacity;
    size_t      relTableCapacity;
} NsBytecodeBuilder;

NsBytecodeBuilder*  nsCreateBytecodeBuilder(void);
void                nsDestroyBytecodeBuilder(NsBytecodeBuilder* builder);
NsBytecode*         nsExtractBytecode(NsBytecodeBuilder* builder);

void    nsEmitBytecode8(NsBytecodeBuilder* builder, uint8_t value);
void    nsEmitBytecode16(NsBytecodeBuilder* builder, uint16_t value);
void    nsEmitBytecode32(NsBytecodeBuilder* builder, uint32_t value);
void    nsEmitBytecode64(NsBytecodeBuilder* builder, uint64_t value);
void    nsEmitBytecodeReg(NsBytecodeBuilder* builder, uint16_t value);
void    nsEmitBytecodeString(NsBytecodeBuilder* builder, const char* str, size_t length);
void    nsEmitBytecodeStringC(NsBytecodeBuilder* builder, const char* str);

uint32_t nsBytecodeInternString(NsBytecodeBuilder* builder, const char* str, size_t length);

#endif
