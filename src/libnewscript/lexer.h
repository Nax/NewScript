#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>
#include <libnewscript/blob.h>

typedef enum {
    NS_TOKEN_EOF,
    NS_TOKEN_LPAREN,
    NS_TOKEN_RPAREN,
    NS_TOKEN_LBRACE,
    NS_TOKEN_RBRACE,
    NS_TOKEN_ASSIGN,
    NS_TOKEN_DOT,
    NS_TOKEN_COMMA,
    NS_TOKEN_COLON,
    NS_TOKEN_SEMICOLON,
    NS_TOKEN_IDENTIFIER,
    NS_TOKEN_STRING,
    NS_TOKEN_KCONST,
    NS_TOKEN_KLET
} NsTokenType;

typedef struct {
    NsTokenType     type;
    union {
        NsBlob      str;
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
