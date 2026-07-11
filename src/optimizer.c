#include "optimizer.h"

#include "optimizer/constant_fold.h"
#include "optimizer/identity.h"
#include "optimizer/algebraic.h"
#include "optimizer/function_fold.h"

ASTNode *optimize(ASTNode *node)
{
    if (node == NULL)
        return NULL;

    /*
        First recursively optimize children.
    */
    switch (node->type)
    {
    case NODE_BINARY:

        node->binary.left =
            optimize(node->binary.left);

        node->binary.right =
            optimize(node->binary.right);

        break;


    case NODE_UNARY:

        node->unary.child =
            optimize(node->unary.child);

        break;


    case NODE_CALL:

        for (size_t i = 0; i < node->call.argc; i++)
        {
            node->call.args[i] =
                optimize(node->call.args[i]);
        }

        break;


    case NODE_ASSIGN:

        node->assign.value =
            optimize(node->assign.value);

        break;


    case NODE_NUMBER:
    case NODE_IDENTIFIER:
    default:
        break;
    }

    /*
        Then run optimization passes.

        Some passes replace the node entirely,
        so only continue if it is still of the
        expected type.
    */

    switch (node->type)
    {
    case NODE_BINARY:

        node = fold_binary(node);

        if (node->type == NODE_BINARY)
            node = fold_identity(node);

        if (node->type == NODE_BINARY)
            node = fold_algebraic(node);

        break;


    case NODE_UNARY:

        node = fold_unary(node);

        break;


    case NODE_CALL:

        node = fold_function(node);

        break;


    default:
        break;
    }

    return node;
}