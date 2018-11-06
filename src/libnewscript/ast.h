#ifndef AST_H
#define AST_H

#include <libnewscript/blob.h>

typedef enum {
    NS_AST_ROOT,
    NS_AST_STATEMENT_EMPTY,
    NS_AST_EXPR_IDENTIFIER,
    NS_AST_EXPR_STRING,
    NS_AST_EXPR_PROPERTY,
    NS_AST_EXPR_CALL
} NsAstType;

typedef struct {

} NsAstRoot;

typedef struct {
    
} NsAstIdentifier;

typedef struct {
    char*   data;
    size_t  len;
} NsAstProperty;

typedef struct {
    NsAstType type;
    union {
        NsAstRoot           root;
        NsAstIdentifier     identifier;
    };
} NsAstNode;

NsAstNode*  nsCreateAstRoot(void);
void        nsDestroyAstNode(NsAstNode* node);

#endif
