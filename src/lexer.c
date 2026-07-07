#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "lexer.h"

/* Current position in the source */
static const char *current = NULL;

static int line = 1;
static int column = 1;

static inline char current_char(void)
{
    return *current;
}

static inline char peek_next(void)
{
    return current[1];
}

static inline void advance(void)
{
    if (*current == '\0')
        return;

    if (*current == '\n')
    {
        line++;
        column = 1;
    }
    else
    {
        column++;
    }

    current++;
}

static inline void skip_whitespace(void)
{
    while (isspace((unsigned char)current_char()))
        advance();
}

static Token make_token(TokenType type, const char *text,
                        int token_line, int token_column)
{
    Token token = {0};

    token.type = type;
    token.line = token_line;
    token.column = token_column;

    strncpy(token.text, text, sizeof(token.text) - 1);
    token.text[sizeof(token.text) - 1] = '\0';

    return token;
}

static Token read_number(void)
{
    char buffer[64];

    int idx = 0;

    int start_line = line;
    int start_column = column;

    int seen_decimal = 0;

    while (1)
    {
        char c = current_char();

        if (isdigit((unsigned char)c))
        {
            if (idx < (int)sizeof(buffer) - 1)
                buffer[idx++] = c;

            advance();
        }
        else if (c == '.' && !seen_decimal)
        {
            seen_decimal = 1;

            if (idx < (int)sizeof(buffer) - 1)
                buffer[idx++] = c;

            advance();
        }
        else
        {
            break;
        }
    }

    buffer[idx] = '\0';

    Token token =
        make_token(TOKEN_NUMBER, buffer,
                   start_line,
                   start_column);

    token.number = strtod(buffer, NULL);

    return token;
}

static Token read_identifier(void)
{
    char buffer[64];

    int idx = 0;

    int start_line = line;
    int start_column = column;

    while (isalnum((unsigned char)current_char()) ||
           current_char() == '_')
    {
        if (idx < (int)sizeof(buffer) - 1)
            buffer[idx++] = current_char();

        advance();
    }

    buffer[idx] = '\0';

    return make_token(TOKEN_IDENTIFIER,
                      buffer,
                      start_line,
                      start_column);
}

void lexer_init(const char *source)
{
    current = source;

    line = 1;
    column = 1;
}

Token lexer_next(void)
{
    skip_whitespace();

    int start_line = line;
    int start_column = column;

    char c = current_char();

    if (c == '\0')
        return make_token(TOKEN_EOF, "",
                          start_line,
                          start_column);

    if (isdigit((unsigned char)c))
        return read_number();

    if (isalpha((unsigned char)c) || c == '_')
        return read_identifier();

    switch (c)
    {
    case '+':
        advance();
        return make_token(TOKEN_PLUS, "+",
                          start_line,
                          start_column);

    case '-':
        advance();
        return make_token(TOKEN_MINUS, "-",
                          start_line,
                          start_column);

    case '*':
        advance();
        return make_token(TOKEN_STAR, "*",
                          start_line,
                          start_column);

    case '/':
        advance();
        return make_token(TOKEN_SLASH, "/",
                          start_line,
                          start_column);

    case '^':
        advance();
        return make_token(TOKEN_POWER, "^",
                          start_line,
                          start_column);

    case '(':
        advance();
        return make_token(TOKEN_LPAREN, "(",
                          start_line,
                          start_column);

    case ')':
        advance();
        return make_token(TOKEN_RPAREN, ")",
                          start_line,
                          start_column);

    case '=':
        if (peek_next() == '=')
        {
            advance();
            advance();
            return make_token(TOKEN_EQUAL, "==",
                              start_line,
                              start_column);
        }

        advance();
        return make_token(TOKEN_ASSIGN, "=",
                          start_line,
                          start_column);

    case '!':
        if (peek_next() == '=')
        {
            advance();
            advance();
            return make_token(TOKEN_NOT_EQUAL, "!=",
                              start_line,
                              start_column);
        }
        break;

    case '<':
        if (peek_next() == '=')
        {
            advance();
            advance();
            return make_token(TOKEN_LESS_EQUAL, "<=",
                              start_line,
                              start_column);
        }

        advance();
        return make_token(TOKEN_LESS, "<",
                          start_line,
                          start_column);

    case '>':
        if (peek_next() == '=')
        {
            advance();
            advance();
            return make_token(TOKEN_GREATER_EQUAL, ">=",
                              start_line,
                              start_column);
        }

        advance();
        return make_token(TOKEN_GREATER, ">",
                          start_line,
                          start_column);

    case ',':
        advance();
        return make_token(TOKEN_COMMA, ",",
                          start_line,
                          start_column);
    }

    char unknown[2] = { c, '\0' };

    advance();

    return make_token(TOKEN_UNKNOWN,
                      unknown,
                      start_line,
                      start_column);
}