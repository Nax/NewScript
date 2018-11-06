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

static void parseLoop(NsParser* parser)
{
    for (;;)
    {
        if (acceptImmediate(parser, NS_TOKEN_EOF))
            return;

        for (;;)
        {
            if (!acceptImmediate(parser, NS_TOKEN_SEMICOLON))
                break;
        }
    }
}

NsAstNode* nsParse(const char* data, size_t len)
{
    NsParser parser;
    parser.lexer = nsCreateLexer(data, len);
    parser.ast = nsCreateAstRoot();
    parser.lookahead = NULL;
    parseLoop(&parser);
    nsDestroyLexer(parser.lexer);
    return parser.ast;
}

