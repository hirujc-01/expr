#ifndef CONSTANT_FOLD_H
#define CONSTANT_FOLD_H

#include "ast.h"

ASTNode *fold_binary(ASTNode *node);
ASTNode *fold_unary(ASTNode *node);
ASTNode *fold_constant(ASTNode *node);

#endif