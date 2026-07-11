#include "optimizer/identity.h"
#include "optimizer/common.h"

ASTNode *fold_identity(ASTNode *node)
{
    if (node->type != NODE_BINARY)
        return node;

    ASTNode *left = node->binary.left;
    ASTNode *right = node->binary.right;

    switch (node->binary.op)
    {
    case OP_ADD:

        if (is_number_value(left, 0))
            return replace_with_child(node, right);

        if (is_number_value(right, 0))
            return replace_with_child(node, left);

        break;

    case OP_SUB:

        if (is_number_value(right, 0))
            return replace_with_child(node, left);

        break;

    case OP_MUL:

        if (is_number_value(left, 0) ||
            is_number_value(right, 0))
        {
            return replace_with_number(node, 0);
        }

        if (is_number_value(left, 1))
            return replace_with_child(node, right);

        if (is_number_value(right, 1))
            return replace_with_child(node, left);

        break;

    case OP_DIV:

        if (is_number_value(left, 0))
            return replace_with_number(node, 0);

        if (is_number_value(right, 1))
            return replace_with_child(node, left);

        break;

    case OP_POW:

        if (is_number_value(right, 1))
            return replace_with_child(node, left);

        if (is_number_value(right, 0))
            return replace_with_number(node, 1);

        if (is_number_value(left, 1))
            return replace_with_number(node, 1);

        if (is_number_value(left, 0) &&
            !is_number_value(right, 0))
        {
            return replace_with_number(node, 0);
        }

        break;

    default:
        break;
    }

    return node;
}