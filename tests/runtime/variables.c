#include "test_commons.h"
#include "runtime_helpers.h"

static int test_assignment(void)
{
    RuntimeSession session;

    runtime_session_init(&session);

    ASSERT_TRUE(runtime_eval_equals(&session,
                                    "x=5",
                                    5));

    ASSERT_TRUE(runtime_eval_equals(&session,
                                    "x",
                                    5));

    runtime_session_free(&session);

    return 1;
}

static int test_reassignment(void)
{
    RuntimeSession session;

    runtime_session_init(&session);

    ASSERT_TRUE(runtime_eval_equals(&session,
                                    "x=5",
                                    5));

    ASSERT_TRUE(runtime_eval_equals(&session,
                                    "x=9",
                                    9));

    ASSERT_TRUE(runtime_eval_equals(&session,
                                    "x",
                                    9));

    runtime_session_free(&session);

    return 1;
}

static int test_multiple_variables(void)
{
    RuntimeSession session;

    runtime_session_init(&session);

    ASSERT_TRUE(runtime_eval_equals(&session,
                                    "a=2",
                                    2));

    ASSERT_TRUE(runtime_eval_equals(&session,
                                    "b=3",
                                    3));

    ASSERT_TRUE(runtime_eval_equals(&session,
                                    "a+b",
                                    5));

    runtime_session_free(&session);

    return 1;
}

void runtime_variable_tests(void)
{
    run_test("assignment", test_assignment);
    run_test("reassignment", test_reassignment);
    run_test("multiple variables", test_multiple_variables);
}