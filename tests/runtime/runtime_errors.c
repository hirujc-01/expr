#include "test_commons.h"
#include "runtime_helpers.h"

static int test_division_by_zero(void)
{
    ASSERT_TRUE(eval_fails("1/0"));
    return 1;
}

static int test_unknown_variable(void)
{
    ASSERT_TRUE(eval_fails("x"));
    return 1;
}

static int test_unknown_function(void)
{
    ASSERT_TRUE(eval_fails("abc(5)"));
    return 1;
}

static int test_invalid_argument_count(void)
{
    ASSERT_TRUE(eval_fails("sqrt()"));
    ASSERT_TRUE(eval_fails("pow(2)"));
    return 1;
}

void runtime_error_tests(void)
{
    run_test("division by zero", test_division_by_zero);
    run_test("unknown variable", test_unknown_variable);
    run_test("unknown function", test_unknown_function);
    run_test("invalid arg count", test_invalid_argument_count);
}