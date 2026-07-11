#include "test_commons.h"
#include "lexer_helpers.h"
#include "lexer.h"

static int test_unknown_characters(void)
{
    ASSERT_TOKEN("@", TOKEN_UNKNOWN, "@");
    ASSERT_TOKEN("#", TOKEN_UNKNOWN, "#");
    ASSERT_TOKEN("$", TOKEN_UNKNOWN, "$");
    ASSERT_TOKEN("&", TOKEN_UNKNOWN, "&");
    ASSERT_TOKEN("`", TOKEN_UNKNOWN, "`");

    return 1;
}

static int test_multiple_unknowns(void)
{
    lexer_init("@#$");

    Token t;

    t = lexer_next();
    ASSERT_TRUE(t.type == TOKEN_UNKNOWN);

    t = lexer_next();
    ASSERT_TRUE(t.type == TOKEN_UNKNOWN);

    t = lexer_next();
    ASSERT_TRUE(t.type == TOKEN_UNKNOWN);

    t = lexer_next();
    ASSERT_TRUE(t.type == TOKEN_EOF);

    return 1;
}

int run_error_tests(void)
{
    int ok = 1;

    ok &= run_test("Lexer: unknown characters", test_unknown_characters);
    ok &= run_test("Lexer: multiple unknown characters", test_multiple_unknowns);

    return ok;
}