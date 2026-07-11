#include <string.h>

#include "test_commons.h"
#include "lexer_helpers.h"


static int test_single_identifier(void)
{
    Token token = get_token("x");

    ASSERT_TRUE(token.type == TOKEN_IDENTIFIER);
    ASSERT_TRUE(strcmp(token.text, "x") == 0);

    return 1;
}


static int test_word_identifier(void)
{
    Token token = get_token("variable");

    ASSERT_TRUE(token.type == TOKEN_IDENTIFIER);
    ASSERT_TRUE(strcmp(token.text, "variable") == 0);

    return 1;
}


static int test_identifier_with_numbers(void)
{
    Token token = get_token("abc123");

    ASSERT_TRUE(token.type == TOKEN_IDENTIFIER);
    ASSERT_TRUE(strcmp(token.text, "abc123") == 0);

    return 1;
}


static int test_underscore_identifier(void)
{
    Token token = get_token("_value");

    ASSERT_TRUE(token.type == TOKEN_IDENTIFIER);
    ASSERT_TRUE(strcmp(token.text, "_value") == 0);

    return 1;
}


static int test_function_name(void)
{
    Token token = get_token("sqrt");

    ASSERT_TRUE(token.type == TOKEN_IDENTIFIER);
    ASSERT_TRUE(strcmp(token.text, "sqrt") == 0);

    return 1;
}


static int test_identifier_followed_by_number(void)
{
    lexer_init("abc123");

    Token token = lexer_next();

    ASSERT_TRUE(token.type == TOKEN_IDENTIFIER);
    ASSERT_TRUE(strcmp(token.text, "abc123") == 0);

    return 1;
}


static int test_separated_identifiers(void)
{
    lexer_init("x y");

    Token first = lexer_next();
    Token second = lexer_next();

    ASSERT_TRUE(first.type == TOKEN_IDENTIFIER);
    ASSERT_TRUE(strcmp(first.text, "x") == 0);

    ASSERT_TRUE(second.type == TOKEN_IDENTIFIER);
    ASSERT_TRUE(strcmp(second.text, "y") == 0);

    return 1;
}


void lexer_identifier_tests(void)
{
    run_test("identifier single",
             test_single_identifier);

    run_test("identifier word",
             test_word_identifier);

    run_test("identifier with numbers",
             test_identifier_with_numbers);

    run_test("identifier underscore",
             test_underscore_identifier);

    run_test("function name identifier",
             test_function_name);

    run_test("identifier followed by number",
             test_identifier_followed_by_number);

    run_test("separated identifiers",
             test_separated_identifiers);
}