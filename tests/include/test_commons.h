#ifndef TEST_COMMONS_H
#define TEST_COMMONS_H

#include <stdio.h>

int run_test(const char *name,
             int (*test)(void));

void print_summary(void);

#define ASSERT_TRUE(expr)                         \
    do                                            \
    {                                             \
        if (!(expr))                              \
        {                                         \
            fprintf(stderr,                       \
                    "Assertion failed: %s\n",     \
                    #expr);                       \
            return 0;                             \
        }                                         \
    } while (0)

#endif