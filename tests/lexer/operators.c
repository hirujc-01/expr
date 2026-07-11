#include "test_commons.h"
#include "lexer_helpers.h"

static int test_arithmetic_operators(void)
{
    ASSERT_TOKEN("+", TOKEN_PLUS, "+");
    ASSERT_TOKEN("-", TOKEN_MINUS, "-");
    ASSERT_TOKEN("*", TOKEN_STAR, "*");
    ASSERT_TOKEN("/", TOKEN_SLASH, "/");
    ASSERT_TOKEN("^", TOKEN_POWER, "^");

    return 1;
}

static int test_parentheses_and_comma(void)
{
    ASSERT_TOKEN("(", TOKEN_LPAREN, "(");
    ASSERT_TOKEN(")", TOKEN_RPAREN, ")");
    ASSERT_TOKEN(",", TOKEN_COMMA, ",");

    return 1;
}

static int test_assignment_and_comparison(void)
{
    ASSERT_TOKEN("=", TOKEN_ASSIGN, "=");

    ASSERT_TOKEN("==", TOKEN_EQUAL, "==");
    ASSERT_TOKEN("!=", TOKEN_NOT_EQUAL, "!=");

    ASSERT_TOKEN("<", TOKEN_LESS, "<");
    ASSERT_TOKEN("<=", TOKEN_LESS_EQUAL, "<=");

    ASSERT_TOKEN(">", TOKEN_GREATER, ">");
    ASSERT_TOKEN(">=", TOKEN_GREATER_EQUAL, ">=");

    return 1;
}

static int test_operator_sequence(void)
{
    lexer_init("+-*/^");

    ASSERT_TRUE(lexer_next().type == TOKEN_PLUS);
    ASSERT_TRUE(lexer_next().type == TOKEN_MINUS);
    ASSERT_TRUE(lexer_next().type == TOKEN_STAR);
    ASSERT_TRUE(lexer_next().type == TOKEN_SLASH);
    ASSERT_TRUE(lexer_next().type == TOKEN_POWER);
    ASSERT_TRUE(lexer_next().type == TOKEN_EOF);

    return 1;
}

int run_operator_tests(void)
{
    int ok = 1;

    ok &= run_test(
        "Lexer: arithmetic operators",
        test_arithmetic_operators);

    ok &= run_test(
        "Lexer: parentheses and comma",
        test_parentheses_and_comma);

    ok &= run_test(
        "Lexer: assignment and comparison",
        test_assignment_and_comparison);

    ok &= run_test(
        "Lexer: operator sequence",
        test_operator_sequence);

    return ok;
}