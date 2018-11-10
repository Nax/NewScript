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

typedef struct NsAstNode_ NsAstNode;

typedef struct {

} NsAstRoot;

typedef struct {
    NsBlob  name;
} NsAstExprIdentifier;

typedef struct {
    NsAstNode*  expr;
    NsBlob      name;
} NsAstExprProperty;

typedef struct {
    size_t      argCount;
    NsAstNode*  function;
    NsAstNode** args;
} NsAstExprCall;

struct NsAstNode_ {
    NsAstType type;
    union {
        NsAstRoot           root;
        NsAstExprIdentifier identifier;
        NsAstExprProperty   property;
        NsAstExprCall       call;
    };
};

NsAstNode*  nsCreateAstRoot(void);
void        nsDestroyAstNode(NsAstNode* node);

#endif
