#ifndef PANIC_H
#define PANIC_H

#include <stdlib.h>

#define panic(...)                                                             \
    {                                                                          \
        fprintf(stderr, "\033[31mError: \033[39m\n");                          \
        fprintf(stderr, __VA_ARGS__);                                          \
        fprintf(stderr, "\n");                                                 \
        exit(1);                                                               \
    }

#define panic_if(cond, ...)                                                    \
    if (cond) {                                                                \
        panic(__VA_ARGS__);                                                    \
    }

#endif
