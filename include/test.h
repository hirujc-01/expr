#ifndef TEST_H
#define TEST_H

int run_test(const char *name,
             int (*test)(void));

void print_summary(void);

#define ASSERT_TRUE(expr)                     \
    do                                       \
    {                                        \
        if (!(expr))                         \
            return 0;                        \
    } while (0)

#endif