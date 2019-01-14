#ifndef INCLUDE_test_h__
#define INCLUDE_test_h__

#include "../common.h"

static int SUCCESS_COUNT = 0;
static int FAIL_COUNT = 0;

#ifdef TEST

#define test_success(M, ...)                                 \
    {                                                        \
        fprintf(stderr, "[\e[0;32mSUCCESS\e[0m] " M "\n", ##__VA_ARGS__); \
        SUCCESS_COUNT += 1;                                  \
    }

#define test_fail(M, ...)                                       \
    {                                                           \
        fprintf(stderr, "\e[0;31m*** FAIL ***\e[0m " M "\n", ##__VA_ARGS__); \
        FAIL_COUNT += 1;                                        \
    }

#define test(A, M, ...)                 \
    if (A)                              \
    {                                   \
        test_success(M, ##__VA_ARGS__); \
    }                                   \
    else                                \
    {                                   \
        test_fail(M, ##__VA_ARGS__);    \
    }

#else

#define test_success(M, ...)
#define test_fail(M, ...)
#define test(A, M, ...)

#endif // TEST

int successes()
{
    return SUCCESS_COUNT;
}

int failures()
{
    return FAIL_COUNT;
}

int report_results()
{
    fprintf(stderr, "Test results: [%d successes, %d failures]\n", SUCCESS_COUNT, FAIL_COUNT);
    return FAIL_COUNT > 0 ? FAIL_COUNT : 0;
}

#endif //INCLUDE_test_h__