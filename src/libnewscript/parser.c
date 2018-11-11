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

static int parseExpr(NsParser* parser)
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
        else
            break;
    }
    return 1;
}

static int parseStatement(NsParser* parser)
{
    parseExpr(parser);
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

void nsParse(const char* data, size_t len)
{
    NsParser parser;
    parser.lexer = nsCreateLexer(data, len);
    parser.builder = nsCreateBytecodeBuilder();
    parser.lookahead = NULL;
    parser.reg = 0;
    parseLoop(&parser);
    nsDestroyLexer(parser.lexer);
    nsDumpBytecode(parser.builder);
    nsDestroyBytecodeBuilder(parser.builder);
}
