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


/*=========================================================
 * Function table (generated from X-macro)
 *========================================================*/

static const FunctionInfo functions[] =
{
#define X(name, min, max) \
    { #name, min, max },

    FUNCTION_LIST

#undef X
};


/*=========================================================
 * Function lookup
 *========================================================*/

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


/*=========================================================
 * AST semantic checker
 *========================================================*/

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


    case NODE_ASSIGN:
        /*
            Assignment:
                x = expression

            Target must be an identifier.
            Value must be valid.
        */

        if (node->assign.target->type != NODE_IDENTIFIER)
        {
            fprintf(stderr,
                    "Left side of assignment must be a variable\n");

            return 0;
        }


        return check_node(node->assign.value);



    default:
        return 0;
    }
}


/*=========================================================
 * Public API
 *========================================================*/

int semantic_check(ASTNode *node)
{
    return check_node(node);
}