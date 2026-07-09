#include <math.h>

#include "test.h"

#include "parser.h"
#include "semantic.h"
#include "evaluator.h"
#include "symbol_table.h"

#define EPSILON 1e-9

static int eval_expr(const char *expr,
                     SymbolTable *symbols,
                     double *result)
{
    ASTNode *tree = parse(expr);

    if (tree == NULL)
        return 0;

    if (!semantic_check(tree))
    {
        ast_free(tree);
        return 0;
    }

    int ok = evaluate(tree,
                      symbols,
                      result);

    ast_free(tree);

    return ok;
}

/*=========================================================
 * 2+3 = 5
 *========================================================*/

int test_basic_add(void)
{
    SymbolTable symbols;
    symbol_init(&symbols);

    double result;

    ASSERT_TRUE(eval_expr("2+3",
                          &symbols,
                          &result));

    ASSERT_TRUE(fabs(result - 5.0) < EPSILON);

    symbol_free(&symbols);

    return 1;
}

/*=========================================================
 * 2+3*4 = 14
 *========================================================*/

int test_precedence(void)
{
    SymbolTable symbols;
    symbol_init(&symbols);

    double result;

    ASSERT_TRUE(eval_expr("2+3*4",
                          &symbols,
                          &result));

    ASSERT_TRUE(fabs(result - 14.0) < EPSILON);

    symbol_free(&symbols);

    return 1;
}

/*=========================================================
 * x=5
 * x+2 = 7
 *========================================================*/

int test_assignment(void)
{
    SymbolTable symbols;
    symbol_init(&symbols);

    double result;

    ASSERT_TRUE(eval_expr("x=5",
                          &symbols,
                          &result));

    ASSERT_TRUE(eval_expr("x+2",
                          &symbols,
                          &result));

    ASSERT_TRUE(fabs(result - 7.0) < EPSILON);

    symbol_free(&symbols);

    return 1;
}

/*=========================================================
 * 2^5 = 32
 *========================================================*/

int test_power(void)
{
    SymbolTable symbols;
    symbol_init(&symbols);

    double result;

    ASSERT_TRUE(eval_expr("2^5",
                          &symbols,
                          &result));

    ASSERT_TRUE(fabs(result - 32.0) < EPSILON);

    symbol_free(&symbols);

    return 1;
}

/*=========================================================
 * sqrt(81) = 9
 *========================================================*/

int test_sqrt(void)
{
    SymbolTable symbols;
    symbol_init(&symbols);

    double result;

    ASSERT_TRUE(eval_expr("sqrt(81)",
                          &symbols,
                          &result));

    ASSERT_TRUE(fabs(result - 9.0) < EPSILON);

    symbol_free(&symbols);

    return 1;
}