#ifndef PARSER_H
#define PARSER_H

#include <libnewscript/bytecode/builder.h>
#include <libnewscript/lexer.h>

typedef struct {
    char*       var;
    uint16_t    reg;
    uint8_t     isConst;
} NsParserBind;

typedef struct {
    size_t          size;
    size_t          capacity;
    NsParserBind*   bindings;
} NsParserVars;

typedef struct {
    NsBytecodeBuilder*  builder;
    NsLexer*            lexer;
    NsToken*            lookahead;
    uint16_t            reg;
    NsParserVars*       vars;
} NsParser;

void nsParse(const char* data, size_t len);

#endif
