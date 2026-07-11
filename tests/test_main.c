#include "test_commons.h"

#include "runtime_tests.h"
#include "lexer_tests.h"
#include "parser_tests.h"
#include "optimizer_tests.h"

int main(void)
{
    run_runtime_tests();
    run_lexer_tests();
    run_parser_tests();
    run_optimizer_tests();

    print_summary();

    return 0;
}