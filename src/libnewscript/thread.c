#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <libnewscript/thread.h>

typedef struct {
    uint32_t            cursor;
    const NsBytecode*   bc;
} NsBytecodeReader;

extern uint64_t kNsThreadTable[];

static void writeThreadData(NsThreadBuilder* builder, const void* data, size_t length)
{
    size_t newCapacity;

    while (builder->size + length > builder->capacity)
    {
        newCapacity = builder->capacity * 2;
        builder->data = realloc(builder->data, newCapacity);
        builder->capacity = newCapacity;
    }

    memcpy((char*)(builder->data) + builder->size, data, length);
    builder->size += length;
}

static void writeThread8(NsThreadBuilder* builder, uint8_t value)
{
    writeThreadData(builder, &value, 1);
}

static void writeThread16(NsThreadBuilder* builder, uint16_t value)
{
    writeThreadData(builder, &value, 2);
}

static void writeThread32(NsThreadBuilder* builder, uint32_t value)
{
    writeThreadData(builder, &value, 4);
}

static void writeThread64(NsThreadBuilder* builder, uint64_t value)
{
    writeThreadData(builder, &value, 8);
}

static void readBytecodeData(void* dst, NsBytecodeReader* reader, size_t length)
{
    memcpy(dst, reader->bc->code + reader->cursor, length);
    reader->cursor += length;
}

static uint8_t readBytecode8(NsBytecodeReader* reader)
{
    uint8_t tmp;
    readBytecodeData(&tmp, reader, 1);
    return tmp;
}

static uint16_t readBytecode16(NsBytecodeReader* reader)
{
    uint16_t tmp;
    readBytecodeData(&tmp, reader, 2);
    return tmp;
}

static uint32_t readBytecode32(NsBytecodeReader* reader)
{
    uint32_t tmp;
    readBytecodeData(&tmp, reader, 4);
    return tmp;
}

static uint64_t readBytecode64(NsBytecodeReader* reader)
{
    uint64_t tmp;
    readBytecodeData(&tmp, reader, 8);
    return tmp;
}

static uint16_t readBytecodeReg(NsBytecodeReader* reader)
{
    uint16_t reg = readBytecode8(reader);
    if (reg & 0x80)
    {
        reg &= 0x7f;
        reg |= (readBytecode8(reader) << 7);
    }
    return reg;
}

void* nsThreadBuild(NsVirtualMachine* vm, NsBytecode* bc)
{
    NsBytecodeReader    reader;
    NsThreadBuilder     builder;
    uint8_t op;

    reader.bc = bc;
    reader.cursor = 0;

    builder.size = 0;
    builder.capacity = 128;
    builder.data = malloc(builder.capacity);

    for (;;)
    {
        if (i >= bc->codeSize)
            break;

        op = bc->code[i++];
        writeThread64(&builder, kNsThreadTable[op]);
        printf("THREAD_OP: 0x%016llx\n", kNsThreadTable[op]);
    }

    return realloc(builder.data, builder.size);
}
