#include <math.h>

#include "runtime_helpers.h"

#include "ast.h"
#include "evaluator.h"
#include "optimizer.h"
#include "parser.h"
#include "symbol_table.h"

#define EPSILON 1e-9

void runtime_session_init(RuntimeSession *session)
{
    symbol_init(&session->symbols);
}

void runtime_session_free(RuntimeSession *session)
{
    symbol_free(&session->symbols);
}

int runtime_eval(RuntimeSession *session,
                 const char *expr,
                 double *result)
{
    ASTNode *tree = parse(expr);

    if (tree == NULL)
        return 0;

    tree = optimize(tree);

    int ok =
        evaluate(tree,
                 &session->symbols,
                 result);

    ast_free(tree);

    return ok;
}

int runtime_eval_equals(RuntimeSession *session,
                        const char *expr,
                        double expected)
{
    double result;

    if (!runtime_eval(session,
                      expr,
                      &result))
    {
        return 0;
    }

    return fabs(result - expected) < EPSILON;
}