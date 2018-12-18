#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <libnewscript/bytecode/op.h>
#include <libnewscript/bytecode/bytecode.h>

void nsDestroyBytecode(NsBytecode* bc)
{
    free(bc->code);
    free(bc->strTableHeader);
    free(bc->strTable);
    free(bc->relTable);
    free(bc);
}

void nsDumpBytecode(NsBytecode* bc)
{
    size_t nameLen;
    size_t cursor = 0;
    size_t relCursor = 0;
    uint8_t enc;
    uint8_t op;
    uint8_t tmp8;
    uint16_t tmp16;
    uint32_t tmp32;
    uint64_t tmp64;
    const NsOp* opData;

    for (;;)
    {
        if (cursor >= bc->codeSize)
            return;
        printf("0x%04x: ", (int)cursor);
        op = bc->code[cursor++];
        opData = nsOps + op;
        printf("%s", opData->name);
        nameLen = strlen(opData->name);

        for (size_t i = 0; i < 3; ++i)
        {
            enc = opData->enc[i];
            if (enc == NS_ENC_NONE)
                break;
            if (i == 0)
            {
                for (size_t j = 0; j < 12 - nameLen; ++j)
                    putchar(' ');
            }
            else
                printf(", ");

            if (relCursor < bc->relTableSize && bc->relTable[relCursor].offset == cursor)
            {
                tmp32 = bc->relTable[relCursor++].index;
                tmp32 = bc->strTableHeader[tmp32].offset;
                printf("\"%s\"", bc->strTable + tmp32);
                cursor += 8;
                continue;
            }

            switch (enc)
            {
            case NS_ENC_IMM8:
                tmp8 = *(uint8_t*)(bc->code + cursor);
                printf("0x%02x", tmp8);
                cursor += 1;
                break;
            case NS_ENC_IMM16:
                tmp16 = *(uint16_t*)(bc->code + cursor);
                printf("0x%04x", tmp16);
                cursor += 2;
                break;
            case NS_ENC_IMM32:
                tmp32 = *(uint32_t*)(bc->code + cursor);
                printf("0x%08x", tmp32);
                cursor += 4;
                break;
            case NS_ENC_IMM64:
                tmp64 = *(uint64_t*)(bc->code + cursor);
                printf("0x%016llx", tmp64);
                cursor += 8;
                break;
            case NS_ENC_REG:
                tmp16 = *(uint8_t*)(bc->code + cursor++);
                if (tmp16 & 0x80)
                {
                    tmp16 &= 0x7f;
                    tmp16 |= ((*(uint8_t*)(bc->code + cursor++)) << 7);
                }
                printf("r%u", tmp16);
                break;
            case NS_ENC_CALLARGS:
                tmp8 = *(uint8_t*)(bc->code + cursor++);
                putchar('(');
                for (uint8_t i = 0; i < tmp8; ++i)
                {
                    tmp16 = *(uint8_t*)(bc->code + cursor++);
                    if (tmp16 & 0x80)
                    {
                        tmp16 &= 0x7f;
                        tmp16 |= ((*(uint8_t*)(bc->code + cursor++)) << 7);
                    }
                    printf("r%d", tmp16);
                    if (i + 1 < tmp8)
                        printf(", ");
                }
                putchar(')');
                break;
            }
        }
        printf("\n");
    }
}
