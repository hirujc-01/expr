#include "lexer_helpers.h"

#include "lexer.h"

Token get_token(const char *input)
{
    lexer_init(input);

    return lexer_next();
}


Token get_next_token(void)
{
    return lexer_next();
}
