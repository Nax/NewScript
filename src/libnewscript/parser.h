#ifndef PARSER_H
#define PARSER_H

#include <libnewscript/ast.h>
#include <libnewscript/lexer.h>

typedef struct {
    NsAstNode*  ast;
    NsLexer*    lexer;
    NsToken*    lookahead;
} NsParser;

NsAstNode* nsParse(const char* data, size_t len);

#endif