#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <libnewscript/bytecode/builder.h>
#include <libnewscript/bytecode/op.h>

NsBytecodeBuilder* nsCreateBytecodeBuilder(void)
{
    NsBytecodeBuilder* builder = malloc(sizeof(*builder));
    builder->codeSize = 0;
    builder->codeCapacity = 128;
    builder->code = malloc(builder->codeCapacity);
    return builder;
}

void nsDestroyBytecodeBuilder(NsBytecodeBuilder* builder)
{
    free(builder->code);
    free(builder);
}

static void appendBytecode(NsBytecodeBuilder* builder, void* data, size_t length)
{
    while (builder->codeSize + length > builder->codeCapacity)
    {
        size_t newCapacity = builder->codeCapacity + builder->codeCapacity / 2;
        builder->code = realloc(builder->code, newCapacity);
    }
    memcpy(builder->code + builder->codeSize, data, length);
    builder->codeSize += length;
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

void nsDumpBytecode(NsBytecodeBuilder* builder)
{
    size_t cursor = 0;
    uint8_t enc;
    uint8_t op;
    uint8_t tmp8;
    uint16_t tmp16;
    uint32_t tmp32;
    uint64_t tmp64;
    const NsOp* opData;

    printf("Bytecode Dump:\n");
    for (;;)
    {
        if (cursor >= builder->codeSize)
            return;
        printf("0x%04x: ", (int)cursor);
        op = builder->code[cursor++];
        opData = nsOps + op;
        printf("%s", opData->name);

        for (size_t i = 0; i < 3; ++i)
        {
            enc = opData->enc[i];
            if (enc == NS_ENC_NONE)
                break;
            if (i == 0)
                printf("    ");
            else
                printf(", ");
            switch (enc)
            {
            case NS_ENC_IMM8:
                tmp8 = *(uint8_t*)(builder->code + cursor);
                printf("0x%02x", tmp8);
                cursor += 1;
                break;
            case NS_ENC_IMM16:
                tmp16 = *(uint16_t*)(builder->code + cursor);
                printf("0x%04x", tmp16);
                cursor += 2;
                break;
            case NS_ENC_IMM32:
                tmp32 = *(uint32_t*)(builder->code + cursor);
                printf("0x%08x", tmp32);
                cursor += 4;
                break;
            case NS_ENC_IMM64:
                tmp64 = *(uint64_t*)(builder->code + cursor);
                printf("0x%016llx", tmp64);
                cursor += 8;
                break;
            case NS_ENC_REG:
                tmp16 = *(uint8_t*)(builder->code + cursor++);
                if (tmp16 & 0x80)
                {
                    tmp16 &= 0x7f;
                    tmp16 |= ((*(uint8_t*)(builder->code + cursor++)) << 7);
                }
                printf("r%u", tmp16);
                break;
            }
        }
        printf("\n");
    }
}
