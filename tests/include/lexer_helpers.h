#ifndef LEXER_HELPERS_H
#define LEXER_HELPERS_H

#include "token.h"
#include "lexer.h"
#include <string.h>

Token get_token(const char *input);

Token get_next_token(void);

#define ASSERT_TOKEN(input, expected_type, expected_text) \
    do { \
        lexer_init(input); \
        Token tok = lexer_next(); \
        ASSERT_TRUE(tok.type == expected_type); \
        ASSERT_TRUE(strcmp(tok.text, expected_text) == 0); \
    } while (0)

#endif