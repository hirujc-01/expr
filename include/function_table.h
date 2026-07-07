#ifndef FUNCTION_TABLE_H
#define FUNCTION_TABLE_H

#include <stddef.h>


typedef double (*BuiltinFunction)(const double *args,
                                  size_t argc);


typedef struct
{
    const char *name;

    int min_args;
    int max_args;

    BuiltinFunction function;

} FunctionInfo;


#endif