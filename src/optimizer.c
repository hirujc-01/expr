#include <math.h>
#include <stdlib.h>
#include <string.h>

#include "optimizer.h"

/*=========================================================
 * Helpers
 *========================================================*/

static int is_number(ASTNode *node)
{
    return node != NULL &&
           node->type == NODE_NUMBER;
}

static int is_number_value(ASTNode *node,
                           double value)
{
    return is_number(node) &&
           node->number.value == value;
}

/*
    Frees children of a node
    WITHOUT freeing the node itself.
*/
static void free_children(ASTNode *node)
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
static ASTNode *replace_with_number(ASTNode *node,
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
static ASTNode *replace_with_child(ASTNode *node,
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

static ASTNode *fold_binary(ASTNode *node);
static ASTNode *fold_unary(ASTNode *node);
static ASTNode *fold_identity(ASTNode *node);

static ASTNode *fold_binary(ASTNode *node)
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

static ASTNode *fold_unary(ASTNode *node)
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

static ASTNode *fold_identity(ASTNode *node)
{
    if (node->type != NODE_BINARY)
        return node;

    ASTNode *left = node->binary.left;
    ASTNode *right = node->binary.right;

    switch (node->binary.op)
    {
    /*-----------------------------------------------------
     * Addition
     *----------------------------------------------------*/

    case OP_ADD:

        if (is_number_value(left, 0))
            return replace_with_child(node, right);

        if (is_number_value(right, 0))
            return replace_with_child(node, left);

        break;


    /*-----------------------------------------------------
     * Subtraction
     *----------------------------------------------------*/

    case OP_SUB:

        if (is_number_value(right, 0))
            return replace_with_child(node, left);

        break;


    /*-----------------------------------------------------
     * Multiplication
     *----------------------------------------------------*/

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


    /*-----------------------------------------------------
     * Division
     *----------------------------------------------------*/

    case OP_DIV:

        if (is_number_value(left, 0))
            return replace_with_number(node, 0);

        if (is_number_value(right, 1))
            return replace_with_child(node, left);

        break;


    /*-----------------------------------------------------
     * Power
     *----------------------------------------------------*/

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

ASTNode *optimize(ASTNode *node)
{
    if (node == NULL)
        return NULL;

    switch (node->type)
    {
    case NODE_BINARY:

        node->binary.left =
            optimize(node->binary.left);

        node->binary.right =
            optimize(node->binary.right);

        node = fold_binary(node);

        if (node->type == NODE_BINARY)
            node = fold_identity(node);

        return node;

    case NODE_UNARY:

        node->unary.child =
            optimize(node->unary.child);

        return fold_unary(node);

    case NODE_CALL:
        printf("argc = %zu\n", node->call.argc);
        printf("args = %p\n", (void *)node->call.args); 
        for (size_t i = 0;
             i < node->call.argc;
             i++)
        {
            node->call.args[i] =
                optimize(node->call.args[i]);
        }

        return node;

    case NODE_ASSIGN:

        node->assign.value =
            optimize(node->assign.value);

        return node;

    default:
        return node;
    }
}