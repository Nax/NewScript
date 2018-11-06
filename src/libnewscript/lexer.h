#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>

typedef enum {
    NS_TOKEN_EOF,
    NS_TOKEN_LPAREN,
    NS_TOKEN_RPAREN,
    NS_TOKEN_LBRACE,
    NS_TOKEN_RBRACE,
    NS_TOKEN_DOT,
    NS_TOKEN_IDENTIFIER,
    NS_TOKEN_STRING
} NsTokenType;

typedef struct {
    char*   data;
    size_t  len;
} NsTokenString;

typedef struct {
    NsTokenType         type;
    union {
        NsTokenString   str;
    };
} NsToken;

typedef struct {
    const char*     buffer;
    size_t          length;
    size_t          cursor;
    unsigned int    eof:1;
} NsLexer;

NsLexer*    nsCreateLexer(const char* buffer, size_t length);
void        nsDestroyLexer(NsLexer* lexer);
NsToken*    nsNextToken(NsLexer* lexer);
void        nsFreeToken(NsToken* token);

#endif