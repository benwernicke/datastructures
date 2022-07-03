#include "test.h"

static uint64_t total_tests = 0;
static uint64_t passed_tests = 0;

void test_int_eq(char* desc, int64_t a, int64_t b)
{
    test_bool(desc, a == b);
}

void test_uint_eq(char* desc, uint64_t a, uint64_t b)
{
    test_bool(desc, a == b);
}

#define GREEN "\033[32m"
#define RESET "\033[39m"
#define RED "\033[31m"

void test_total()
{
    printf("passed \t\t%lu/%lu: ", passed_tests, total_tests);
    if (total_tests == passed_tests) {
        printf(GREEN "success\n" RESET);
    } else {
        printf(RED "failed\n" RESET);
    }
}

void test_bool(char* desc, bool b)
{
    total_tests++;
    if (b) {
        printf("%s\t\t: " GREEN "success\n"RESET, desc);
        passed_tests++;
    } else {
        printf("%s\t\t: " RED "failed\n"RESET, desc);
    }
}
