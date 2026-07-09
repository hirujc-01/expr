#include "test.h"

/* evaluator */
int test_basic_add(void);
int test_precedence(void);
int test_assignment(void);
int test_power(void);
int test_sqrt(void);

int main(void)
{
    run_test("basic_add",
             test_basic_add);

    run_test("precedence",
             test_precedence);

    run_test("assignment",
             test_assignment);

    run_test("power",
             test_power);

    run_test("sqrt",
             test_sqrt);

    print_summary();

    return 0;
}