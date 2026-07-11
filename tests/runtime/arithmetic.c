#include <math.h>

#include "test_commons.h"
#include "runtime_helpers.h"

#include "parser.h"
#include "optimizer.h"
#include "evaluator.h"
#include "symbol_table.h"
#include "ast.h"

/*=========================================================
 * Individual tests
 *========================================================*/

static int test_addition(void)
{
    ASSERT_TRUE(eval_equals("1+2", 3));
    return 1;
}

static int test_subtraction(void)
{
    ASSERT_TRUE(eval_equals("7-4", 3));
    return 1;
}

static int test_multiplication(void)
{
    ASSERT_TRUE(eval_equals("6*5", 30));
    return 1;
}

static int test_division(void)
{
    ASSERT_TRUE(eval_equals("12/3", 4));
    return 1;
}

static int test_power(void)
{
    ASSERT_TRUE(eval_equals("2^5", 32));
    return 1;
}

static int test_precedence(void)
{
    ASSERT_TRUE(eval_equals("2+3*4", 14));
    return 1;
}

static int test_parentheses(void)
{
    ASSERT_TRUE(eval_equals("(2+3)*4", 20));
    return 1;
}

static int test_unary_minus(void)
{
    ASSERT_TRUE(eval_equals("-5", -5));
    return 1;
}

static int test_double_negative(void)
{
    ASSERT_TRUE(eval_equals("--5", 5));
    return 1;
}

/*=========================================================
 * Public suite
 *========================================================*/

void runtime_arithmetic_tests(void)
{
    run_test("addition",           test_addition);
    run_test("subtraction",        test_subtraction);
    run_test("multiplication",     test_multiplication);
    run_test("division",           test_division);
    run_test("power",              test_power);
    run_test("precedence",         test_precedence);
    run_test("parentheses",        test_parentheses);
    run_test("unary minus",        test_unary_minus);
    run_test("double negative",    test_double_negative);
}