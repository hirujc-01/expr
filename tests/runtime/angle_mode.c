#include "test_commons.h"
#include "runtime_helpers.h"
#include "angle_mode.h"

static int test_degree_mode(void)
{
    set_angle_mode(ANGLE_DEG);

    ASSERT_TRUE(eval_equals("sin(90)", 1));
    ASSERT_TRUE(eval_equals("cos(180)", -1));

    return 1;
}

static int test_radian_mode(void)
{
    set_angle_mode(ANGLE_RAD);

    ASSERT_TRUE(eval_equals("sin(pi/2)", 1));
    ASSERT_TRUE(eval_equals("cos(pi)", -1));

    return 1;
}

void runtime_angle_tests(void)
{
    run_test("degree mode", test_degree_mode);
    run_test("radian mode", test_radian_mode);
}