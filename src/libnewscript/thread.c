#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <libnewscript/thread.h>

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

    memcpy((char*)(builder->data) + length, data, length);
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

void* nsThreadBuild(NsVirtualMachine* vm, NsBytecode* bc)
{
    NsThreadBuilder builder;
    size_t i;
    uint8_t op;

    builder.size = 0;
    builder.capacity = 128;
    builder.data = malloc(builder.capacity);

    i = 0;
    for (;;)
    {
        if (i >= bc->codeSize)
            break;

        op = bc->code[i++];
        writeThread64(&builder, kNsThreadTable[op]);
    }

    return realloc(builder.data, builder.size);
}
