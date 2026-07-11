#include "runtime_tests.h"

void runtime_arithmetic_tests(void);
void runtime_builtin_tests(void);
void runtime_variable_tests(void);
void runtime_angle_tests(void);
void runtime_error_tests(void);

void run_runtime_tests(void)
{
    runtime_arithmetic_tests();
    runtime_builtin_tests();
    runtime_variable_tests();
    runtime_angle_tests();
    runtime_error_tests();
}