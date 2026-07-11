#include <math.h>

#include "optimizer/function_fold.h"
#include "optimizer/common.h"

#include "builtins.h"

/*=========================================================
 * Helper
 *========================================================*/

int all_constant(ASTNode *node)
{
    for (size_t i = 0; i < node->call.argc; i++)
    {
        if (!is_number(node->call.args[i]))
            return 0;
    }

    return 1;
}

ASTNode *fold_function(ASTNode *node)
{
    if (node->type != NODE_CALL)
        return node;

    if (!all_constant(node))
        return node;

    double args[64];

    for (size_t i = 0; i < node->call.argc; i++)
    {
        args[i] = node->call.args[i]->number.value;
    }

    if (node->call.id >= FUNC_INVALID)
        return node;

    return replace_with_number(
        node,
        builtin_call(node->call.id,
                     args,
                     node->call.argc)
    );
}