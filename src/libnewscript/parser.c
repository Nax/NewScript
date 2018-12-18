#include <string.h>
#include <libnewscript/bytecode/op.h>
#include <libnewscript/parser.h>
#include <libnewscript/thread.h>

static int allocVar(uint16_t* dst, NsParser* parser, const char* name)
{
    NsParserBind* bind = parser->vars->bindings + parser->vars->size++;
    bind->var = strdup(name);
    bind->reg = parser->reg++;
    bind->isConst = 0;
    *dst = bind->reg;
    return 1;
}

static int allocVarFixed(NsParser* parser, const char* name, uint16_t src)
{
    NsParserBind* bind = parser->vars->bindings + parser->vars->size++;
    bind->var = strdup(name);
    bind->reg = src;
    bind->isConst = 0;
    return 1;
}

static int seekVar(uint16_t* dst, NsParser* parser, const char* name)
{
    for (size_t i = 0; i < parser->vars->size; ++i)
    {
        if (strcmp(parser->vars->bindings[i].var, name) == 0)
        {
            *dst = parser->vars->bindings[i].reg;
            return 1;
        }
    }
    return 0;
}

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
    if (seekVar(&reg, parser, tok->str.data))
    {
        *dst = reg;
        nsFreeToken(tok);
        return 1;
    }
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
            nsEmitBytecode8(parser->builder, NS_OP_CALL);
            nsEmitBytecodeReg(parser->builder, reg);
            nsEmitBytecodeReg(parser->builder, 0);
            nsEmitBytecode8(parser->builder, argCount);
            for (size_t i = 0; i < argCount; ++i)
                nsEmitBytecodeReg(parser->builder, args[i]);
            reg = 0;
        }
        else
            break;
    }
    *dst = reg;
    return 1;
}

static int parseVarDecl(NsParser* parser)
{
    NsToken* identifier;
    uint16_t expr;

    if (!acceptImmediate(parser, NS_TOKEN_KLET) && !acceptImmediate(parser, NS_TOKEN_KCONST))
        return 0;
    identifier = accept(parser, NS_TOKEN_IDENTIFIER);
    acceptImmediate(parser, NS_TOKEN_ASSIGN);
    parseExpr(&expr, parser);
    allocVarFixed(parser, identifier->str.data, expr);
    nsFreeToken(identifier);
    return 1;
}

static int parseStatement(NsParser* parser)
{
    int ret;
    uint16_t dummy;

    ret = parseVarDecl(parser);
    if (!ret)
        ret = parseExpr(&dummy, parser);
    return acceptImmediate(parser, NS_TOKEN_SEMICOLON);
}

#include <stdio.h>
#include <stdlib.h>

static void parseLoop(NsParser* parser)
{
    for (;;)
    {
        if (acceptImmediate(parser, NS_TOKEN_EOF))
            break;

        if (!parseStatement(parser))
        {
            printf("Fatal Error\n");
            exit(1);
        }
    }
    nsEmitBytecode8(parser->builder, NS_OP_RETNIL);
    parser->builder->bc->regCount = parser->reg;
}

#include <libnewscript/vm/vm.h>
#include <libnewscript/vm/string.h>

void nsParse(const char* data, size_t len)
{
    NsBytecode* bc;
    NsParser parser;
    NsParserVars* vars;
    void* thread;

    parser.lexer = nsCreateLexer(data, len);
    parser.builder = nsCreateBytecodeBuilder();
    parser.lookahead = NULL;
    parser.reg = 1;

    vars = malloc(sizeof(*vars));
    vars->size = 0;
    vars->capacity = 16;
    vars->bindings = malloc(vars->capacity * sizeof(*vars->bindings));
    parser.vars = vars;

    parseLoop(&parser);
    nsDestroyLexer(parser.lexer);

    bc = nsExtractBytecode(parser.builder);
    printf("===  PARSE  ===\n\n");
    nsDumpBytecode(bc);

    NsVirtualMachine* vm;
    vm = nsCreateVirtualMachine();
    printf("\n===  LINK   ===\n\n");
    nsVmLinkBytecode(vm, bc);
    nsDumpBytecode(bc);
    printf("\n===  EXEC   ===\n\n");
    thread = nsThreadBuild(vm, bc);
    nsRunThread(vm, thread);
    nsDestroyVirtualMachine(vm);
    nsDestroyBytecode(bc);
}
