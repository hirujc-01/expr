#include <math.h>

#include "builtins.h"
#include "angle_mode.h"

/*=========================================================
 * Builtin functions
 *========================================================*/

static double eval_builtin_sin(const double *args,
                               size_t argc)
{
    (void)argc;
    return sin(to_radians(args[0]));
}


static double eval_builtin_cos(const double *args,
                               size_t argc)
{
    (void)argc;
    return cos(to_radians(args[0]));
}


static double eval_builtin_tan(const double *args,
                               size_t argc)
{
    (void)argc;
    return tan(to_radians(args[0]));
}


static double eval_builtin_sqrt(const double *args,
                                size_t argc)
{
    (void)argc;
    return sqrt(args[0]);
}


static double eval_builtin_pow(const double *args,
                               size_t argc)
{
    (void)argc;
    return pow(args[0], args[1]);
}


static double eval_builtin_max(const double *args,
                               size_t argc)
{
    if (argc == 0)
        return NAN;


    double result = args[0];


    for (size_t i = 1; i < argc; i++)
    {
        if (args[i] > result)
            result = args[i];
    }


    return result;
}


static double eval_builtin_min(const double *args,
                               size_t argc)
{
    if (argc == 0)
        return NAN;


    double result = args[0];


    for (size_t i = 1; i < argc; i++)
    {
        if (args[i] < result)
            result = args[i];
    }


    return result;
}


/*=========================================================
 * Function table
 *========================================================*/

const FunctionInfo functions[] =
{
#define X(name, min, max) \
    { #name, min, max, eval_builtin_##name },

    FUNCTION_LIST

#undef X
};

double builtin_call(FunctionID id,
                    const double *args,
                    size_t argc)
{
    if (id >= FUNC_INVALID)
        return NAN;

    return functions[id].function(args, argc);
}