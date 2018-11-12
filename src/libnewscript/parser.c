#include <libnewscript/bytecode/op.h>
#include <libnewscript/parser.h>

static NsToken* accept(NsParser* parser, NsTokenType type)
{
    NsToken* token;

    token = parser->lookahead;
    if (!token)
        token = nsNextToken(parser->lexer);
    if (!token)
        return NULL;
    if (token->type == type)
    {
        parser->lookahead = NULL;
        return token;
    }
    parser->lookahead = token;
    return NULL;
}

static int acceptImmediate(NsParser* parser, NsTokenType type)
{
    NsToken* tok;

    tok = accept(parser, type);
    if (!tok)
        return 0;
    nsFreeToken(tok);
    return 1;
}

static int parseExprLiteralString(uint16_t* dst, NsParser* parser)
{
    NsToken* tok;
    uint16_t reg;

    tok = accept(parser, NS_TOKEN_STRING);
    if (!tok)
        return 0;
    reg = parser->reg++;
    nsEmitBytecode8(parser->builder, NS_OP_LOADI);
    nsEmitBytecodeReg(parser->builder, reg);
    nsEmitBytecodeString(parser->builder, tok->str.data, tok->str.size);
    nsFreeToken(tok);
    *dst = reg;
    return 1;
}

static int parseExprIdentifier(uint16_t* dst, NsParser* parser)
{
    NsToken* tok;
    uint16_t reg;

    tok = accept(parser, NS_TOKEN_IDENTIFIER);
    if (!tok)
        return 0;
    reg = parser->reg++;
    nsEmitBytecode8(parser->builder, NS_OP_GGETI);
    nsEmitBytecodeReg(parser->builder, reg);
    nsEmitBytecodeString(parser->builder, tok->str.data, tok->str.size);
    nsFreeToken(tok);
    *dst = reg;
    return 1;
}

static int parseExpr(uint16_t* dst, NsParser* parser)
{
    NsToken* tok;
    uint16_t tmp;
    uint16_t reg;
    uint16_t args[256];
    uint8_t argCount;
    int ret;

    ret = parseExprIdentifier(&reg, parser);
    if (!ret)
        ret = parseExprLiteralString(&reg, parser);
    if (!ret)
        return 0;

    for (;;)
    {
        if (acceptImmediate(parser, NS_TOKEN_DOT))
        {
            tok = accept(parser, NS_TOKEN_IDENTIFIER);
            nsEmitBytecode8(parser->builder, NS_OP_GETI);
            nsEmitBytecodeReg(parser->builder, reg);
            nsEmitBytecodeReg(parser->builder, reg);
            nsEmitBytecodeString(parser->builder, tok->str.data, tok->str.size);
            nsFreeToken(tok);
        }
        else if(acceptImmediate(parser, NS_TOKEN_LPAREN))
        {
            argCount = 0;
            if (parseExpr(&tmp, parser))
            {
                args[0] = tmp;
                argCount = 1;

                for (;;)
                {
                    if (acceptImmediate(parser, NS_TOKEN_COMMA))
                    {
                        parseExpr(&tmp, parser);
                        args[argCount++] = tmp;
                    }
                    else
                        break;
                }
            }
            acceptImmediate(parser, NS_TOKEN_RPAREN);
            if (argCount == 0)
                args[0] = parser->reg++;
            nsEmitBytecode8(parser->builder, NS_OP_CALL);
            nsEmitBytecodeReg(parser->builder, reg);
            nsEmitBytecodeReg(parser->builder, args[0]);
            nsEmitBytecode8(parser->builder, argCount);
            reg = args[0];
        }
        else
            break;
    }
    *dst = reg;
    return 1;
}

static int parseStatement(NsParser* parser)
{
    uint16_t dummy;

    parseExpr(&dummy, parser);
    return acceptImmediate(parser, NS_TOKEN_SEMICOLON);
}

static void parseLoop(NsParser* parser)
{
    for (;;)
    {
        if (acceptImmediate(parser, NS_TOKEN_EOF))
            return;

        parseStatement(parser);
    }
}

#include <stdio.h>
#include <libnewscript/vm/vm.h>
#include <libnewscript/vm/string.h>

void nsParse(const char* data, size_t len)
{
    NsBytecode* bc;
    NsParser parser;
    parser.lexer = nsCreateLexer(data, len);
    parser.builder = nsCreateBytecodeBuilder();
    parser.lookahead = NULL;
    parser.reg = 0;
    parseLoop(&parser);
    nsDestroyLexer(parser.lexer);

    bc = nsExtractBytecode(parser.builder);
    nsDumpBytecode(bc);

    NsVirtualMachine* vm;
    vm = nsCreateVirtualMachine();
    printf("\n=== LINK ===\n\n");
    nsVmLinkBytecode(vm, bc);
    nsDumpBytecode(bc);
    nsDestroyVirtualMachine(vm);
    nsDestroyBytecode(bc);
}
