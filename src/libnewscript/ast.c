#include <stdlib.h>
#include <libnewscript/ast.h>

NsAstNode* nsCreateAstRoot(void)
{
    NsAstNode* node;
    node = malloc(sizeof(*node));
    node->type = NS_AST_ROOT;
    return node;
}

void nsDestroyAstNode(NsAstNode* node)
{
    if (!node)
        return;

    free(node);
}
