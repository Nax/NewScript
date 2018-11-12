#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <libnewscript/bytecode/builder.h>
#include <libnewscript/bytecode/op.h>

NsBytecodeBuilder* nsCreateBytecodeBuilder(void)
{
    NsBytecodeBuilder* builder = malloc(sizeof(*builder));
    NsBytecode* bc = malloc(sizeof(*builder->bc));

    builder->bc = bc;
    builder->codeCapacity = 128;
    builder->strTableHeaderCapacity = 8;
    builder->strTableCapacity = 64;
    builder->relTableCapacity = 8;

    bc->codeSize = 0;
    bc->code = malloc(builder->codeCapacity);
    bc->strTableHeaderSize = 0;
    bc->strTableHeader = malloc(sizeof(*bc->strTableHeader) * builder->strTableHeaderCapacity);
    bc->strTableSize = 0;
    bc->strTable = malloc(builder->strTableCapacity);
    bc->relTableSize = 0;
    bc->relTable = malloc(sizeof(*bc->relTable) * builder->relTableCapacity);
    bc->regCount = 0;
    return builder;
}

void nsDestroyBytecodeBuilder(NsBytecodeBuilder* builder)
{
    NsBytecode* bc;

    bc = builder->bc;
    if (bc)
        nsDestroyBytecode(bc);
    free(builder);
}

NsBytecode* nsExtractBytecode(NsBytecodeBuilder* builder)
{
    NsBytecode* bc;

    bc = builder->bc;
    bc->code = realloc(bc->code, bc->codeSize);
    bc->strTableHeader = realloc(bc->strTableHeader, bc->strTableHeaderSize * sizeof(*bc->strTableHeader));
    bc->strTable = realloc(bc->strTable, bc->strTableSize);
    bc->relTable = realloc(bc->relTable, bc->relTableSize * sizeof(*bc->relTable));
    builder->bc = NULL;
    nsDestroyBytecodeBuilder(builder);
    return bc;
}

static void appendBytecode(NsBytecodeBuilder* builder, void* data, size_t length)
{
    while (builder->bc->codeSize + length > builder->codeCapacity)
    {
        size_t newCapacity = builder->codeCapacity + builder->codeCapacity / 2;
        builder->bc->code = realloc(builder->bc->code, newCapacity);
        builder->codeCapacity = newCapacity;
    }
    memcpy(builder->bc->code + builder->bc->codeSize, data, length);
    builder->bc->codeSize += length;
}

void nsEmitBytecode8(NsBytecodeBuilder* builder, uint8_t value)
{
    appendBytecode(builder, &value, 1);
}

void nsEmitBytecode16(NsBytecodeBuilder* builder, uint16_t value)
{
    appendBytecode(builder, &value, 2);
}

void nsEmitBytecode32(NsBytecodeBuilder* builder, uint32_t value)
{
    appendBytecode(builder, &value, 4);
}

void nsEmitBytecode64(NsBytecodeBuilder* builder, uint64_t value)
{
    appendBytecode(builder, &value, 8);
}

void nsEmitBytecodeReg(NsBytecodeBuilder* builder, uint16_t value)
{
    if (value < 128)
        nsEmitBytecode8(builder, value);
    else
    {
        nsEmitBytecode8(builder, (value & 0x7f) | 0x80);
        nsEmitBytecode8(builder, (value >> 7) & 0xff);
    }
}

void nsEmitBytecodeString(NsBytecodeBuilder* builder, const char* str, size_t length)
{
    uint32_t strIndex;
    uint32_t relIndex;
    size_t newCapacity;

    strIndex = nsBytecodeInternString(builder, str, length);
    if (builder->bc->relTableSize == builder->relTableCapacity)
    {
        newCapacity = builder->relTableCapacity + builder->relTableCapacity / 2;
        builder->bc->relTable = realloc(builder->bc->relTable, newCapacity * sizeof(*builder->bc->relTable));
        builder->relTableCapacity = newCapacity;
    }
    relIndex = builder->bc->relTableSize;
    builder->bc->relTable[relIndex].offset = builder->bc->codeSize;
    builder->bc->relTable[relIndex].index = strIndex;
    builder->bc->relTable[relIndex].type = NS_REL_STRING;
    builder->bc->relTableSize++;
    nsEmitBytecode64(builder, 0);
}

void nsEmitBytecodeStringC(NsBytecodeBuilder* builder, const char* str)
{
    nsEmitBytecodeString(builder, str, strlen(str));
}

uint32_t nsBytecodeInternString(NsBytecodeBuilder* builder, const char* str, size_t length)
{
    size_t newCapacity;
    uint32_t offset;
    uint32_t index;

    for (size_t i = 0; i < builder->bc->strTableHeaderSize; ++i)
    {
        if (builder->bc->strTableHeader[i].length != length)
            continue;

        if (memcmp(str, builder->bc->strTable + builder->bc->strTableHeader[i].offset, length) == 0)
            return (uint32_t)i;
    }

    while (length + builder->bc->strTableSize + 1 > builder->strTableCapacity)
    {
        newCapacity = builder->strTableCapacity + builder->strTableCapacity / 2;
        builder->bc->strTable = realloc(builder->bc->strTable, newCapacity);
        builder->strTableCapacity = newCapacity;
    }

    offset = builder->bc->strTableSize;
    memcpy(builder->bc->strTable + offset, str, length);
    builder->bc->strTable[offset + length] = 0;
    builder->bc->strTableSize += (length + 1);

    if (builder->bc->strTableHeaderSize == builder->strTableHeaderCapacity)
    {
        newCapacity = builder->strTableHeaderCapacity + builder->strTableHeaderCapacity / 2;
        builder->bc->strTableHeader = realloc(builder->bc->strTableHeader, newCapacity * sizeof(*builder->bc->strTableHeader));
        builder->strTableHeaderCapacity = newCapacity;
    }

    index = builder->bc->strTableHeaderSize;
    builder->bc->strTableHeader[index].offset = offset;
    builder->bc->strTableHeader[index].length = (uint32_t)length;
    builder->bc->strTableHeaderSize += 1;

    return index;
}
