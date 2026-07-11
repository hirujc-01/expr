#include "optimizer/common.h"

#include <stdlib.h>

/*=========================================================
 * Helpers
 *========================================================*/

int is_number(ASTNode *node)
{
    return node != NULL &&
           node->type == NODE_NUMBER;
}

int is_number_value(ASTNode *node,
                           double value)
{
    return is_number(node) &&
           node->number.value == value;
}

/*
    Frees children of a node
    WITHOUT freeing the node itself.
*/
void free_children(ASTNode *node)
{
    if (node == NULL)
        return;

    switch (node->type)
    {
    case NODE_BINARY:

        ast_free(node->binary.left);
        ast_free(node->binary.right);

        node->binary.left = NULL;
        node->binary.right = NULL;

        break;

    case NODE_UNARY:

        ast_free(node->unary.child);

        node->unary.child = NULL;

        break;

    case NODE_CALL:

        for (size_t i = 0;
             i < node->call.argc;
             i++)
        {
            ast_free(node->call.args[i]);
        }

        free(node->call.args);

        node->call.args = NULL;
        node->call.argc = 0;

        break;

    case NODE_ASSIGN:

        ast_free(node->assign.target);
        ast_free(node->assign.value);

        node->assign.target = NULL;
        node->assign.value = NULL;

        break;

    default:
        break;
    }
}

/*
    Replace any subtree with a constant number.

    The node itself is reused.
*/
ASTNode *replace_with_number(ASTNode *node,
                                    double value)
{
    free_children(node);

    node->type = NODE_NUMBER;
    node->number.value = value;

    return node;
}

/*
    Replace a node with one of its children.

    This is the ONLY safe way to return
    an existing child.
*/
ASTNode *replace_with_child(ASTNode *node,
                                   ASTNode *child)
{
    ASTNode *left = NULL;
    ASTNode *right = NULL;

    if (node->type == NODE_BINARY)
    {
        left = node->binary.left;
        right = node->binary.right;

        node->binary.left = NULL;
        node->binary.right = NULL;
    }

    else if (node->type == NODE_UNARY)
    {
        left = node->unary.child;
        node->unary.child = NULL;
    }

    if (left != NULL &&
        left != child)
    {
        ast_free(left);
    }

    if (right != NULL &&
        right != child)
    {
        ast_free(right);
    }

    free(node);

    return child;
}