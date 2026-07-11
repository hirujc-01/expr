#ifndef OPTIMIZER_COMMON_H
#define OPTIMIZER_COMMON_H

#include "ast.h"

int is_number(ASTNode *node);

int is_number_value(ASTNode *node,
                    double value);

void free_children(ASTNode *node);

ASTNode *replace_with_number(ASTNode *node,
                             double value);

ASTNode *replace_with_child(ASTNode *node,
                            ASTNode *child);

#endif