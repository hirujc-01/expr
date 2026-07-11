#include <string.h>

#include "optimizer/ast_equal.h"

int ast_equal(const ASTNode *a, const ASTNode *b)
{
    if (a == b)
        return 1;

    if (a == NULL || b == NULL)
        return 0;

    if (a->type != b->type)
        return 0;

    switch (a->type)
    {
    case NODE_NUMBER:
        return a->number.value == b->number.value;

    case NODE_IDENTIFIER:
        return strcmp(a->identifier.name,
                      b->identifier.name) == 0;

    case NODE_UNARY:
        return a->unary.op == b->unary.op &&
               ast_equal(a->unary.child,
                         b->unary.child);

    case NODE_BINARY:
        return a->binary.op == b->binary.op &&
               ast_equal(a->binary.left,
                         b->binary.left) &&
               ast_equal(a->binary.right,
                         b->binary.right);

    case NODE_ASSIGN:
        return ast_equal(a->assign.target,
                         b->assign.target) &&
               ast_equal(a->assign.value,
                         b->assign.value);

    case NODE_CALL:
    {
        if (a->call.id != b->call.id)
            return 0;

        if (a->call.argc != b->call.argc)
            return 0;

        for (size_t i = 0; i < a->call.argc; i++)
        {
            if (!ast_equal(a->call.args[i],
                           b->call.args[i]))
                return 0;
        }

        return 1;
    }
    }

    return 0;
}