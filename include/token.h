#ifndef TOKEN_H
#define TOKEN_H
#include "token_list.h"

typedef enum
{
#define X(name) name,
    TOKEN_LIST
#undef X
} TokenType;

typedef struct
{
    TokenType type;

    /* Original text of the token */
    char text[64];

    /* Numeric value (only valid for TOKEN_NUMBER) */
    double number;

    /* Position in the source */
    int line;
    int column;

} Token;

void token_print(Token token);

/* Utilities */
const char *token_name(TokenType type);

#endif // TOKEN_H