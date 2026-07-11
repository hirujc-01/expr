#include "test_commons.h"
#include "lexer_helpers.h"

#include <string.h>

static int test_builtin_functions(void)
{
    ASSERT_TOKEN("sin",  TOKEN_IDENTIFIER, "sin");
    ASSERT_TOKEN("cos",  TOKEN_IDENTIFIER, "cos");
    ASSERT_TOKEN("tan",  TOKEN_IDENTIFIER, "tan");
    ASSERT_TOKEN("asin", TOKEN_IDENTIFIER, "asin");
    ASSERT_TOKEN("acos", TOKEN_IDENTIFIER, "acos");
    ASSERT_TOKEN("atan", TOKEN_IDENTIFIER, "atan");

    ASSERT_TOKEN("sqrt", TOKEN_IDENTIFIER, "sqrt");
    ASSERT_TOKEN("pow",  TOKEN_IDENTIFIER, "pow");

    ASSERT_TOKEN("ln",   TOKEN_IDENTIFIER, "ln");
    ASSERT_TOKEN("log",  TOKEN_IDENTIFIER, "log");

    ASSERT_TOKEN("exp",  TOKEN_IDENTIFIER, "exp");

    ASSERT_TOKEN("abs",  TOKEN_IDENTIFIER, "abs");

    return 1;
}

static int test_function_like_names(void)
{
    ASSERT_TOKEN("sin123", TOKEN_IDENTIFIER, "sin123");
    ASSERT_TOKEN("sqrt2",  TOKEN_IDENTIFIER, "sqrt2");
    ASSERT_TOKEN("pow_",   TOKEN_IDENTIFIER, "pow_");

    return 1;
}

int run_function_tests(void)
{
    int ok = 1;

    ok &= run_test("Lexer: builtin functions", test_builtin_functions);
    ok &= run_test("Lexer: function-like identifiers", test_function_like_names);

    return ok;
}