#include "test_commons.h"
#include "lexer_helpers.h"

#include <string.h>

static int test_integer(void)
{
    Token token = get_token("123");

    ASSERT_TRUE(token.type == TOKEN_NUMBER);
    ASSERT_TRUE(token.number == 123);

    return 1;
}


static int test_decimal(void)
{
    Token token = get_token("3.14");

    ASSERT_TRUE(token.type == TOKEN_NUMBER);
    ASSERT_TRUE(token.number > 3.13);
    ASSERT_TRUE(token.number < 3.15);

    return 1;
}


static int test_zero(void)
{
    Token token = get_token("0");

    ASSERT_TRUE(token.type == TOKEN_NUMBER);
    ASSERT_TRUE(token.number == 0);

    return 1;
}


void lexer_number_tests(void)
{
    run_test("lexer integer", test_integer);
    run_test("lexer decimal", test_decimal);
    run_test("lexer zero", test_zero);
}