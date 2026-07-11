#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "parser.h"
#include "lexer.h"


/*=========================================================
 * Parser state
 *========================================================*/

static Token current;

static int parse_error = 0;


/*=========================================================
 * Forward declarations
 *========================================================*/

static void next_token(void);

static int expect(TokenType type);

static ASTNode *expression(void);
static ASTNode *term(void);
static ASTNode *factor(void);
static ASTNode *unary(void);
static ASTNode *primary(void);

static ASTNode *function_call(FunctionID id);
static ASTNode *assignment(void);


/*=========================================================
 * Utilities
 *========================================================*/

static void next_token(void)
{
    current = lexer_next();
}

static void lowercase(char *str)
{
    while (*str)
    {
        *str = (char)tolower((unsigned char)*str);
        str++;
    }
}

static int expect(TokenType type)
{
    if (current.type != type)
    {
        fprintf(stderr,
                "Parse error at %d:%d: expected %s, found %s\n",
                current.line,
                current.column,
                token_name(type),
                token_name(current.type));

        parse_error = 1;
        return 0;
    }

    next_token();

    return 1;
}

static FunctionID lookup_function(const char *name)
{
#define X(fname,min,max) \
    if (strcmp(name, #fname) == 0) \
        return FUNC_##fname;

    FUNCTION_LIST

#undef X

    return FUNC_INVALID;
}

/*=========================================================
 * Primary expressions
 *========================================================*/

static ASTNode *primary(void)
{
    if (parse_error)
        return NULL;


    /* Numbers */
    if (current.type == TOKEN_NUMBER)
    {
        ASTNode *node = ast_number(current.number);

        next_token();

        return node;
    }


    /* Identifiers and function calls */
    if (current.type == TOKEN_IDENTIFIER)
    {
        char name[64];

        strncpy(name,
                current.text,
                sizeof(name) - 1);

        name[sizeof(name) - 1] = '\0';

        lowercase(name);

        name[sizeof(name) - 1] = '\0';


        next_token();


        /*
         * identifier followed by '('
         * means function call
         */
        if (current.type == TOKEN_LPAREN)
        {
            FunctionID id = lookup_function(name);

            if (id == FUNC_INVALID)
            {
                fprintf(stderr,
                        "Unknown function '%s'\n",
                        name);

                parse_error = 1;
                return NULL;
            }

            return function_call(id);
        }

        return ast_identifier(name);
    }


    /* Parenthesized expression */
    if (current.type == TOKEN_LPAREN)
    {
        next_token();

        ASTNode *node = expression();

        if (!expect(TOKEN_RPAREN))
        {
            ast_free(node);
            return NULL;
        }

        return node;
    }


    /* Error */
    if (current.type == TOKEN_EOF)
    {
        fprintf(stderr,
                "Unexpected end of input at %d:%d\n",
                current.line,
                current.column);
    }
    else
    {
        fprintf(stderr,
                "Unexpected token '%s' at %d:%d\n",
                current.text,
                current.line,
                current.column);
    }

    parse_error = 1;

    return NULL;
}


/*=========================================================
 * Function calls
 *========================================================*/

static ASTNode *function_call(FunctionID id)
{
    /*
        Current token is '('
    */

    expect(TOKEN_LPAREN);


    size_t capacity = 4;
    size_t argc = 0;

    ASTNode **args = malloc(sizeof(ASTNode *) * capacity);

    if (args == NULL)
    {
        fprintf(stderr,
                "Fatal: Out of memory.\n");

        exit(EXIT_FAILURE);
    }


    /*
        foo()
        Empty argument list
    */
    if (current.type != TOKEN_RPAREN)
    {
        while (1)
        {
            ASTNode *arg = expression();


            if (arg == NULL)
            {
                for (size_t i = 0; i < argc; i++)
                    ast_free(args[i]);

                free(args);

                return NULL;
            }


            if (argc >= capacity)
            {
                capacity *= 2;

                args = realloc(args,
                               sizeof(ASTNode *) * capacity);

                if (args == NULL)
                {
                    fprintf(stderr,
                            "Fatal: Out of memory.\n");

                    exit(EXIT_FAILURE);
                }
            }


            args[argc++] = arg;


            /*
                More arguments?
            */
            if (current.type == TOKEN_COMMA)
            {
                next_token();
                continue;
            }

            break;
        }
    }


    if (!expect(TOKEN_RPAREN))
    {
        for (size_t i = 0; i < argc; i++)
            ast_free(args[i]);

        free(args);

        return NULL;
    }


    return ast_call(id,
                    args,
                    argc);
}

/*=========================================================
 * Unary operators
 *========================================================*/

static ASTNode *unary(void)
{
    if (current.type == TOKEN_PLUS)
    {
        next_token();

        /*
            Unary plus does nothing.
        */
        return unary();
    }

    if (current.type == TOKEN_MINUS)
    {
        next_token();

        ASTNode *child = unary();

        if (child == NULL)
            return NULL;

        return ast_unary(OP_NEG, child);
    }

    return primary();
}

/*=========================================================
 * Power
 *========================================================*/

static ASTNode *factor(void)
{
    ASTNode *left = unary();

    if (left == NULL)
        return NULL;


    /*
        Right associative:
        2^3^4 = 2^(3^4)
    */
    if (current.type == TOKEN_POWER)
    {
        next_token();

        ASTNode *right = factor();

        if (right == NULL)
        {
            ast_free(left);
            return NULL;
        }

        return ast_binary(OP_POW,
                          left,
                          right);
    }


    return left;
}


/*=========================================================
 * Multiplication / Division
 *========================================================*/

static int starts_implicit_mul(TokenType type)
{
    return type == TOKEN_IDENTIFIER ||
           type == TOKEN_LPAREN;
}


static ASTNode *term(void)
{
    ASTNode *left = factor();

    if (left == NULL)
        return NULL;


    while (1)
    {
        TokenType op = current.type;


        /*
            Explicit multiplication/division
        */
        if (op == TOKEN_STAR ||
            op == TOKEN_SLASH)
        {
            next_token();

            ASTNode *right = factor();

            if (right == NULL)
            {
                ast_free(left);
                return NULL;
            }


            if (op == TOKEN_STAR)
                left = ast_binary(OP_MUL,
                                  left,
                                  right);
            else
                left = ast_binary(OP_DIV,
                                  left,
                                  right);

            continue;
        }


        /*
            Implicit multiplication

            Examples:
                2x
                2(x+1)
                xsin(y)
        */
        if (starts_implicit_mul(current.type))
        {
            ASTNode *right = factor();

            if (right == NULL)
            {
                ast_free(left);
                return NULL;
            }


            left = ast_binary(OP_MUL,
                              left,
                              right);

            continue;
        }


        break;
    }


    return left;
}


/*=========================================================
 * Addition / Subtraction
 *========================================================*/

static ASTNode *expression(void)
{
    ASTNode *left = term();

    if (left == NULL)
        return NULL;


    while (current.type == TOKEN_PLUS ||
           current.type == TOKEN_MINUS)
    {
        TokenType op = current.type;

        next_token();


        ASTNode *right = term();

        if (right == NULL)
        {
            ast_free(left);
            return NULL;
        }


        if (op == TOKEN_PLUS)
        {
            left = ast_binary(OP_ADD,
                              left,
                              right);
        }
        else
        {
            left = ast_binary(OP_SUB,
                              left,
                              right);
        }
    }


    return left;
}

/*=========================================================
 * Variable assignment
 *========================================================*/

static ASTNode *assignment(void)
{
    ASTNode *left = expression();

    if (current.type == TOKEN_ASSIGN)
    {
        next_token();

        ASTNode *right = assignment();

        return ast_assign(left, right);
    }

    return left;
}

/*=========================================================
 * Public parser entry
 *========================================================*/

ASTNode *parse(const char *input)
{
    parse_error = 0;


    lexer_init(input);

    next_token();

    ASTNode *tree = assignment();


    if (parse_error)
    {
        ast_free(tree);
        return NULL;
    }


    if (!expect(TOKEN_EOF))
    {
        ast_free(tree);
        return NULL;
    }


    return tree;
}