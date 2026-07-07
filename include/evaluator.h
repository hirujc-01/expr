#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "ast.h"
#include "symbol_table.h"

int evaluate(ASTNode *node,
             SymbolTable *symbols,
             double *result);

#endif