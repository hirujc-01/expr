#ifndef BUILTINS_H
#define BUILTINS_H

#include "function_table.h"

double builtin_call(FunctionID id,
                    const double *args,
                    size_t argc);

extern const FunctionInfo functions[];

#endif