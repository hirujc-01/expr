#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ast.h"
#include "operator_list.h"
#include "node_list.h"

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

/*=========================================================
 * Name tables
 *========================================================*/

static const char *operator_names[] =
{
#define X(name, symbol) symbol,
    OPERATOR_LIST
#undef X
};

static const char *node_names[] =
{
#define X(name) #name,
    NODE_LIST
#undef X
};

const char *operator_name(Operator op)
{
    if ((unsigned)op >= ARRAY_SIZE(operator_names))
        return "?";

    return operator_names[op];
}

const char *node_name(NodeType type)
{
    if ((unsigned)type >= ARRAY_SIZE(node_names))
        return "?";

    return node_names[type];
}

/*=========================================================
 * Internal helper
 *========================================================*/

static ASTNode *new_node(NodeType type)
{
    ASTNode *node = calloc(1, sizeof(ASTNode));

    if (!node)
    {
        fprintf(stderr, "Fatal: Out of memory.\n");
        exit(EXIT_FAILURE);
    }

    node->type = type;

    return node;
}

/*=========================================================
 * Constructors
 *========================================================*/

ASTNode *ast_number(double value)
{
    ASTNode *node = new_node(NODE_NUMBER);
    node->number.value = value;
    return node;
}

ASTNode *ast_identifier(const char *name)
{
    ASTNode *node = new_node(NODE_IDENTIFIER);

    strncpy(node->identifier.name,
            name,
            sizeof(node->identifier.name) - 1);

    return node;
}

ASTNode *ast_binary(Operator op,
                    ASTNode *left,
                    ASTNode *right)
{
    ASTNode *node = new_node(NODE_BINARY);

    node->binary.op = op;
    node->binary.left = left;
    node->binary.right = right;

    return node;
}

ASTNode *ast_unary(Operator op,
                   ASTNode *child)
{
    ASTNode *node = new_node(NODE_UNARY);

    node->unary.op = op;
    node->unary.child = child;

    return node;
}

ASTNode *ast_call(const char *name,
                  ASTNode **args,
                  size_t argc)
{
    ASTNode *node = new_node(NODE_CALL);

    strncpy(node->call.name,
            name,
            sizeof(node->call.name) - 1);

    node->call.args = args;
    node->call.argc = argc;

    return node;
}

/*=========================================================
 * Debug printing
 *========================================================*/

static void print_node(ASTNode *node)
{
    switch (node->type)
    {
    case NODE_NUMBER:
        printf("%g", node->number.value);
        break;

    case NODE_IDENTIFIER:
        printf("%s", node->identifier.name);
        break;

    case NODE_BINARY:
        printf("%s", operator_name(node->binary.op));
        break;

    case NODE_UNARY:
        printf("%s", operator_name(node->unary.op));
        break;

    case NODE_CALL:
        printf("CALL %s", node->call.name);
        break;

    default:
        printf("<%s>", node_name(node->type));
        break;
    }
}

static void ast_print_impl(ASTNode *node,
                           const char *prefix,
                           int is_last)
{
    if (!node)
        return;

    printf("%s%s", prefix, is_last ? "└── " : "├── ");

    print_node(node);

    putchar('\n');

    char next_prefix[256];

    snprintf(next_prefix,
             sizeof(next_prefix),
             "%s%s",
             prefix,
             is_last ? "    " : "│   ");

    switch (node->type)
    {
    case NODE_BINARY:
        ast_print_impl(node->binary.left,
                       next_prefix,
                       0);

        ast_print_impl(node->binary.right,
                       next_prefix,
                       1);
        break;

    case NODE_UNARY:
        ast_print_impl(node->unary.child,
                       next_prefix,
                       1);
        break;

    case NODE_CALL:
        for (size_t i = 0; i < node->call.argc; ++i)
        {
            ast_print_impl(node->call.args[i],
                           next_prefix,
                           i + 1 == node->call.argc);
        }
        break;

    default:
        break;
    }
}

void ast_print(ASTNode *node, int indent)
{
    (void)indent;

    if (!node)
    {
        puts("(null)");
        return;
    }

    print_node(node);
    putchar('\n');

    switch (node->type)
    {
    case NODE_BINARY:
        ast_print_impl(node->binary.left, "", 0);
        ast_print_impl(node->binary.right, "", 1);
        break;

    case NODE_UNARY:
        ast_print_impl(node->unary.child, "", 1);
        break;

    case NODE_CALL:
        for (size_t i = 0; i < node->call.argc; ++i)
        {
            ast_print_impl(node->call.args[i],
                           "",
                           i + 1 == node->call.argc);
        }
        break;

    default:
        break;
    }
}

/*=========================================================
 * Cleanup
 *========================================================*/

void ast_free(ASTNode *node)
{
    if (!node)
        return;

    switch (node->type)
    {
    case NODE_BINARY:
        ast_free(node->binary.left);
        ast_free(node->binary.right);
        break;

    case NODE_UNARY:
        ast_free(node->unary.child);
        break;

    case NODE_CALL:
        for (size_t i = 0; i < node->call.argc; ++i)
            ast_free(node->call.args[i]);

        free(node->call.args);
        break;

    default:
        break;
    }

    free(node);
}