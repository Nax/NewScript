#ifndef PARSER_H
#define PARSER_H

#include <libnewscript/bytecode/builder.h>
#include <libnewscript/lexer.h>

typedef struct {
    NsBytecodeBuilder*  builder;
    NsLexer*            lexer;
    NsToken*            lookahead;
    uint16_t            reg;
} NsParser;

void nsParse(const char* data, size_t len);

#endif
