#include <stdlib.h>

#include "optimizer/ast_clone.h"

ASTNode *ast_clone(const ASTNode *node)
{
    if (node == NULL)
        return NULL;

    switch (node->type)
    {
    case NODE_NUMBER:
        return ast_number(node->number.value);

    case NODE_IDENTIFIER:
        return ast_identifier(node->identifier.name);

    case NODE_UNARY:
        return ast_unary(
            node->unary.op,
            ast_clone(node->unary.child));

    case NODE_BINARY:
        return ast_binary(
            node->binary.op,
            ast_clone(node->binary.left),
            ast_clone(node->binary.right));

    case NODE_ASSIGN:
        return ast_assign(
            ast_clone(node->assign.target),
            ast_clone(node->assign.value));

    case NODE_CALL:
    {
        ASTNode **args = malloc(sizeof(ASTNode *) * node->call.argc);

        if (args == NULL)
            return NULL;

        for (size_t i = 0; i < node->call.argc; i++)
            args[i] = ast_clone(node->call.args[i]);

        return ast_call(
            node->call.id,
            args,
            node->call.argc);
    }

    default:
        return NULL;

    }

    return NULL;
}