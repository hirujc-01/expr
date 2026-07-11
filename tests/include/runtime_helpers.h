#ifndef RUNTIME_HELPERS_H
#define RUNTIME_HELPERS_H

#include "symbol_table.h"

typedef struct
{
    SymbolTable symbols;
} RuntimeSession;

void runtime_session_init(RuntimeSession *session);
void runtime_session_free(RuntimeSession *session);

int runtime_eval(RuntimeSession *session,
                 const char *expr,
                 double *result);

int runtime_eval_equals(RuntimeSession *session,
                        const char *expr,
                        double expected);

int eval_expression(const char *expr,
                    double *result);

int eval_equals(const char *expr,
                double expected);

int eval_fails(const char *expr);

#endif