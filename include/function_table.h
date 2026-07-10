#ifndef FUNCTION_TABLE_H
#define FUNCTION_TABLE_H

#include <stddef.h>
#include "lists/function_list.h"

typedef double (*BuiltinFunction)(const double *args,
                                  size_t argc);

typedef struct
{
    const char *name;
    int min_args;
    int max_args;
    BuiltinFunction function;

} FunctionInfo;

typedef enum
{
#define X(name,min,max) FUNC_##name,
    FUNCTION_LIST
#undef X

    FUNC_INVALID

} FunctionID;

/* Shared function table */
extern const FunctionInfo functions[];

/* Shared dispatcher */
double builtin_call(FunctionID id,
                    const double *args,
                    size_t argc);

#endif