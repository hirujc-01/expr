#include <stdio.h>

#include "token.h"

static const char *token_names[] =
{
#define X(token) #token,
    TOKEN_LIST
#undef X
};

void token_print(Token token)
{
    printf("[%d:%d] %-15s",
           token.line,
           token.column,
           token_names[token.type]);

    switch (token.type)
    {
    case TOKEN_NUMBER:
        printf(" text=\"%s\" value=%g", token.text, token.number);
        break;

    case TOKEN_IDENTIFIER:
    case TOKEN_UNKNOWN:
        printf(" \"%s\"", token.text);
        break;

    default:
        if (token.text[0] != '\0')
            printf(" \"%s\"", token.text);
        break;
    }

    putchar('\n');
}

const char *token_name(TokenType type)
{
    if (type < 0 || type >= (TokenType)(sizeof(token_names) / sizeof(token_names[0])))
        return "INVALID_TOKEN";

    return token_names[type];
}