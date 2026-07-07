#include <stdio.h>
#include <string.h>

#include "semantic.h"
#include "lists/function_list.h"


typedef struct
{
    const char *name;

    int min_args;
    int max_args;

} FunctionInfo;

#define FUNCTION_LIST \
    X(sin,  1, 1) \
    X(cos,  1, 1) \
    X(tan,  1, 1) \
    X(sqrt, 1, 1) \
    X(pow,  2, 2) \
    X(max,  1, -1) \
    X(min,  1, -1)

static const FunctionInfo *find_function(const char *name)
{
    size_t count =
        sizeof(functions) / sizeof(functions[0]);


    for (size_t i = 0; i < count; i++)
    {
        if (strcmp(functions[i].name, name) == 0)
            return &functions[i];
    }


    return NULL;
}

static int check_node(ASTNode *node)
{
    if (node == NULL)
        return 0;


    switch (node->type)
    {

    case NODE_NUMBER:
        return 1;


    case NODE_IDENTIFIER:
        /*
            Variables are allowed for now.
            Symbol table comes later.
        */
        return 1;


    case NODE_UNARY:
        return check_node(node->unary.child);



    case NODE_BINARY:
        return check_node(node->binary.left) &&
               check_node(node->binary.right);



    case NODE_CALL:
    {
        const FunctionInfo *fn =
            find_function(node->call.name);


        if (fn == NULL)
        {
            fprintf(stderr,
                    "Unknown function '%s'\n",
                    node->call.name);

            return 0;
        }


        if (node->call.argc < (size_t)fn->min_args)
        {
            fprintf(stderr,
                    "%s expects at least %d arguments\n",
                    fn->name,
                    fn->min_args);

            return 0;
        }


        if (fn->max_args != -1 &&
            node->call.argc > (size_t)fn->max_args)
        {
            fprintf(stderr,
                    "%s expects %d arguments\n",
                    fn->name,
                    fn->max_args);

            return 0;
        }


        for (size_t i = 0;
             i < node->call.argc;
             i++)
        {
            if (!check_node(node->call.args[i]))
                return 0;
        }


        return 1;
    }


    default:
        return 0;
    }
}

int semantic_check(ASTNode *node)
{
    return check_node(node);
}