#include "optimizer/common.h"
#include "optimizer/constant_fold.h"
#include <stdlib.h>
#include <math.h>

ASTNode *fold_binary(ASTNode *node)
{
    if (node->type != NODE_BINARY)
        return node;

    ASTNode *left = node->binary.left;
    ASTNode *right = node->binary.right;

    if (!is_number(left) ||
        !is_number(right))
    {
        return node;
    }

    double a = left->number.value;
    double b = right->number.value;

    switch (node->binary.op)
    {
    case OP_ADD:
        return replace_with_number(node, a + b);

    case OP_SUB:
        return replace_with_number(node, a - b);

    case OP_MUL:
        return replace_with_number(node, a * b);

    case OP_DIV:

        /* leave division by zero to evaluator */

        if (b == 0.0)
            return node;

        return replace_with_number(node, a / b);

    case OP_POW:
        return replace_with_number(node, pow(a, b));

    default:
        return node;
    }
}

ASTNode *fold_unary(ASTNode *node)
{
    if (node->type != NODE_UNARY)
        return node;

    ASTNode *child = node->unary.child;

    /*
        Constant folding
    */
    if (is_number(child))
    {
        switch (node->unary.op)
        {
        case OP_NEG:
            return replace_with_number(
                node,
                -child->number.value
            );

        default:
            return node;
        }
    }

    /*
        Identity:
            -(-x) -> x
    */
    if (node->unary.op == OP_NEG &&
        child->type == NODE_UNARY &&
        child->unary.op == OP_NEG)
    {
        ASTNode *grandchild =
            child->unary.child;

        /*
            Detach grandchild so ast_free()
            won't delete it.
        */
        child->unary.child = NULL;

        ast_free(child);

        node->unary.child = NULL;

        free(node);

        return grandchild;
    }

    return node;
}

ASTNode *fold_constant(ASTNode *node)
{
    if (node == NULL)
        return NULL;

    switch (node->type)
    {
    case NODE_BINARY:
        return fold_binary(node);

    case NODE_UNARY:
        return fold_unary(node);

    default:
        return node;
    }
}