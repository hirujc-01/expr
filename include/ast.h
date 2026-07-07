#ifndef AST_H
#define AST_H

#include <stddef.h>

#include "lists/node_list.h"
#include "lists/operator_list.h"

typedef struct ASTNode ASTNode;

/*=========================================================
 * Node types
 *========================================================*/

typedef enum
{
#define X(name) name,
    NODE_LIST
#undef X
} NodeType;

/*=========================================================
 * Operators
 *========================================================*/

typedef enum
{
#define X(name, symbol) name,
    OPERATOR_LIST
#undef X
} Operator;

/*=========================================================
 * AST node
 *========================================================*/

struct ASTNode
{
    NodeType type;

    union
    {
        /* Numeric literal */
        struct
        {
            double value;
        } number;

        /* Variable name */
        struct
        {
            char name[64];
        } identifier;

        /* Binary operator */
        struct
        {
            Operator op;

            ASTNode *left;
            ASTNode *right;

        } binary;

        /* Unary operator */
        struct
        {
            Operator op;

            ASTNode *child;

        } unary;

        /* Function call */
        struct
        {
            char name[64];

            ASTNode **args;
            size_t argc;

        } call;
    };
};

/*=========================================================
 * Constructors
 *========================================================*/

ASTNode *ast_number(double value);

ASTNode *ast_identifier(const char *name);

ASTNode *ast_binary(Operator op,
                    ASTNode *left,
                    ASTNode *right);

ASTNode *ast_unary(Operator op,
                   ASTNode *child);

ASTNode *ast_call(const char *name,
                  ASTNode **args,
                  size_t argc);

/*=========================================================
 * Utilities
 *========================================================*/

const char *operator_name(Operator op);

const char *node_name(NodeType type);

/*=========================================================
 * Debugging
 *========================================================*/

void ast_print(ASTNode *node,
               int indent);

/*=========================================================
 * Cleanup
 *========================================================*/

void ast_free(ASTNode *node);

#endif /* AST_H */