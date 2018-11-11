#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <libnewscript/lexer.h>
#include <libnewscript/bytecode/builder.h>

#include <stdio.h>

NsLexer* nsCreateLexer(const char* buffer, size_t length)
{
    NsLexer* lexer;
    lexer = malloc(sizeof(*lexer));
    lexer->buffer = buffer;
    lexer->length = length;
    lexer->cursor = 0;
    lexer->eof = 0;
    return lexer;
}

void nsDestroyLexer(NsLexer* lexer)
{
    free(lexer);
}

static NsToken* _tokenBasic(NsTokenType type)
{
    NsToken* tok;
    tok = malloc(sizeof(*tok));
    tok->type = type;
    return tok;
}

static NsToken* _tokenString(NsTokenType type, const char* buffer, size_t size)
{
    NsToken* tok;
    tok = _tokenBasic(type);
    nsBlobInitMemory(&tok->str, buffer,  size);
    return tok;
}

static void _skipWhitespace(NsLexer* lexer)
{
    char c;

    while (lexer->cursor < lexer->length)
    {
        c = lexer->buffer[lexer->cursor];
        if (c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f')
            lexer->cursor++;
        else
            break;
    }
}

static NsToken* _match(NsLexer* lexer, const char* data, size_t len, NsTokenType type)
{
    if (lexer->cursor + len > lexer->length)
        return NULL;

    if (memcmp(lexer->buffer + lexer->cursor, data, len) == 0)
    {
        lexer->cursor += len;
        return _tokenBasic(type);
    }

    return NULL;
}

#define MATCH_SYMBOL(sym, type) if ((tok = _match(lexer, sym, sizeof(sym) - 1, type))) { return tok; }
static NsToken* _lexSymbols(NsLexer* lexer)
{
    NsToken* tok;
    MATCH_SYMBOL("(", NS_TOKEN_LPAREN);
    MATCH_SYMBOL(")", NS_TOKEN_RPAREN);
    MATCH_SYMBOL("{", NS_TOKEN_LBRACE);
    MATCH_SYMBOL("}", NS_TOKEN_RBRACE);
    MATCH_SYMBOL(".", NS_TOKEN_DOT);
    MATCH_SYMBOL(",", NS_TOKEN_COMMA);
    MATCH_SYMBOL(":", NS_TOKEN_COLON);
    MATCH_SYMBOL(";", NS_TOKEN_SEMICOLON);
    return tok;
}

static NsToken* _lexIdentifiers(NsLexer* lexer)
{
    NsToken* tok;

    if (!isalpha(lexer->buffer[lexer->cursor]))
        return NULL;

    size_t len = 1;
    while (lexer->cursor + len < lexer->length)
    {
        if (isalnum(lexer->buffer[lexer->cursor + len]))
            len++;
        else
            break;
    }
    tok = _tokenString(NS_TOKEN_IDENTIFIER, lexer->buffer + lexer->cursor, len);
    lexer->cursor += len;
    return tok;
}

static NsToken* _lexStrings(NsLexer* lexer)
{
    NsToken* tok;
    char delim;

    delim = lexer->buffer[lexer->cursor];
    if (delim != '"' && delim != '\'')
        return NULL;

    size_t len = 0;
    while (lexer->cursor + len + 1 < lexer->length)
    {
        if (lexer->buffer[lexer->cursor + len + 1] == delim)
            break;
        len++;
    }
    tok = _tokenString(NS_TOKEN_STRING, lexer->buffer + lexer->cursor + 1, len);
    lexer->cursor += len + 2;
    return tok;
}

NsToken* nsNextToken(NsLexer* lexer)
{
    NsToken* tok;

    if (lexer->eof)
        return NULL;

    _skipWhitespace(lexer);

    if (lexer->cursor == lexer->length)
    {
        lexer->eof = 1;
        return _tokenBasic(NS_TOKEN_EOF);
    }

    if ((tok = _lexSymbols(lexer)))
        return tok;

    if ((tok = _lexIdentifiers(lexer)))
        return tok;

    if ((tok = _lexStrings(lexer)))
        return tok;

    return NULL;
}

void nsFreeToken(NsToken* token)
{
    if (token)
    {
        switch (token->type)
        {
        case NS_TOKEN_IDENTIFIER:
        case NS_TOKEN_STRING:
            nsBlobFree(&token->str);
            break;
        default:
            break;
        }
    }
    free(token);
}

void nsDebugLex(const char* buffer, size_t length)
{
    NsLexer* lexer = nsCreateLexer(buffer, length);
    NsToken* tok;

    while ((tok = nsNextToken(lexer)))
    {
        switch(tok->type)
        {
        case NS_TOKEN_IDENTIFIER:
        case NS_TOKEN_STRING:
            printf("tok: %d (%s)\n", tok->type, tok->str.data);
            break;
        default:
            printf("tok: %d\n", tok->type);
            break;
        }
        nsFreeToken(tok);
    }
    nsDestroyLexer(lexer);
}
