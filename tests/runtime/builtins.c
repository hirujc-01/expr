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

static int test_sqrt(void)
{
    ASSERT_TRUE(eval_equals("sqrt(81)", 9));
    return 1;
}

static int test_pow(void)
{
    ASSERT_TRUE(eval_equals("pow(2,8)", 256));
    return 1;
}

static int test_max(void)
{
    ASSERT_TRUE(eval_equals("max(2,7,3,1)", 7));
    return 1;
}

static int test_min(void)
{
    ASSERT_TRUE(eval_equals("min(2,7,3,1)", 1));
    return 1;
}

static int test_nested_calls(void)
{
    ASSERT_TRUE(eval_equals("sqrt(pow(3,2))", 3));
    return 1;
}

static int test_function_expression(void)
{
    ASSERT_TRUE(eval_equals("sqrt(16)+pow(2,3)", 12));
    return 1;
}

/*=========================================================
 * Public suite
 *========================================================*/

void runtime_builtin_tests(void)
{
    run_test("sqrt()",               test_sqrt);
    run_test("pow()",                test_pow);
    run_test("max()",                test_max);
    run_test("min()",                test_min);
    run_test("nested functions",     test_nested_calls);
    run_test("function expression",  test_function_expression);
}