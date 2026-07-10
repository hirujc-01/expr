#include <math.h>
#include <stdio.h>

#include "evaluator.h"
#include "builtins.h"

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

static int eval_node(ASTNode *node,
                     SymbolTable *symbols,
                     double *result);


/*=========================================================
 * Function call evaluation
 *========================================================*/

static int eval_call(ASTNode *node,
                     SymbolTable *symbols,
                     double *result)
{
    double args[64];


    if (node->call.id >= FUNC_INVALID)
    {
        fprintf(stderr,
                "Unknown function (id=%u)\n",
                (unsigned)node->call.id);
        return 0;
    }

    const FunctionInfo *function =
        &functions[node->call.id];


    if (node->call.argc <
        (size_t)function->min_args)
    {
        fprintf(stderr,
                "%s expects at least %d arguments\n",
                function->name,
                function->min_args);

        return 0;
    }


    if (function->max_args != -1 &&
        node->call.argc >
        (size_t)function->max_args)
    {
        fprintf(stderr,
                "%s expects %d arguments\n",
                function->name,
                function->max_args);

        return 0;
    }


    if (node->call.argc > ARRAY_SIZE(args))
    {
        fprintf(stderr,
                "Too many function arguments\n");

        return 0;
    }


    for (size_t i = 0;
         i < node->call.argc;
         i++)
    {
        if (!eval_node(node->call.args[i],
                       symbols,
                       &args[i]))
        {
            return 0;
        }
    }


    *result =
        builtin_call(node->call.id,
                    args,
                    node->call.argc);


    return 1;
}


/*=========================================================
 * Recursive evaluator
 *========================================================*/

static int eval_node(ASTNode *node,
                     SymbolTable *symbols,
                     double *result)
{
    if (node == NULL)
        return 0;


    switch (node->type)
    {

    case NODE_NUMBER:
        *result = node->number.value;
        return 1;



    case NODE_IDENTIFIER:
    {
        if (symbol_get(symbols,
                    node->identifier.name,
                    result))
        {
            return 1;
        }


        if (constant_get(node->identifier.name,
                        result))
        {
            return 1;
        }


        fprintf(stderr,
                "Undefined variable '%s'\n",
                node->identifier.name);

        return 0;
    }



    case NODE_UNARY:

        if (!eval_node(node->unary.child,
                       symbols,
                       result))
        {
            return 0;
        }


        if (node->unary.op == OP_NEG)
        {
            *result = -*result;
            return 1;
        }


        return 0;



    case NODE_BINARY:
    {
        double left;
        double right;


        if (!eval_node(node->binary.left,
                       symbols,
                       &left) ||
            !eval_node(node->binary.right,
                       symbols,
                       &right))
        {
            return 0;
        }


        switch (node->binary.op)
        {
        case OP_ADD:
            *result = left + right;
            return 1;

        case OP_SUB:
            *result = left - right;
            return 1;

        case OP_MUL:
            *result = left * right;
            return 1;

        case OP_DIV:
            if (right == 0)
            {
                fprintf(stderr,
                        "Division by zero\n");

                return 0;
            }

            *result = left / right;
            return 1;

        case OP_POW:
            *result = pow(left, right);
            return 1;

        default:
            return 0;
        }
    }



    case NODE_CALL:

        return eval_call(node,
                         symbols,
                         result);



    case NODE_ASSIGN:
    {
        if (!eval_node(node->assign.value,
                       symbols,
                       result))
        {
            return 0;
        }


        if (!symbol_set(symbols,
                        node->assign.target->identifier.name,
                        *result))
        {
            return 0;
        }


        return 1;
    }



    default:
        return 0;
    }
}


/*=========================================================
 * Public API
 *========================================================*/

int evaluate(ASTNode *node,
             SymbolTable *symbols,
             double *result)
{
    return eval_node(node,
                     symbols,
                     result);
}
