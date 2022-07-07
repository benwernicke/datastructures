#ifndef TEST_H
#define TEST_H

#include <assert.h>
#include <stdarg.h>
#include <stdio.h>

#define test_(cond, desc) \
    assert(cond);         \
    printf(__func__);     \
    printf(" ");          \
    printf(desc);         \
    printf(": \033[32\t\t\tmsuccess\033[39m\n");

#define test(cond, ...) test_(cond, #cond)

#endif
