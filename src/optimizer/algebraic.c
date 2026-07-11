#include "optimizer/algebraic.h"

#include "optimizer/ast_clone.h"
#include "optimizer/ast_equal.h"

#include "ast.h"

ASTNode *fold_algebraic(ASTNode *node)
{
    if (node == NULL)
        return NULL;

    /*
        x + x -> 2*x
    */
    if (node->type == NODE_BINARY &&
        node->binary.op == OP_ADD &&
        ast_equal(node->binary.left, node->binary.right))
    {

        ASTNode *replacement =
            ast_binary(OP_MUL,
                    ast_number(2),
                    ast_clone(node->binary.left));

        ast_free(node);

        printf("Returning replacement\n");

        return replacement;
    }

    return node;
}