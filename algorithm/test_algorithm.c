#include "../test/test.h"
#include "algorithm.h"
#include <time.h>

#include <stdlib.h>

#define NUM_TESTS 1000

bool is_even(uint64_t* u)
{
    return *u % 2 == 0;
}

void test_remove_if(void)
{
    uint64_t nums[NUM_TESTS];
    uint64_t num;

    uint64_t test_nums[NUM_TESTS];
    uint64_t test_nums_size = 0;
    uint64_t i;
    srand(time(NULL));

    for (i = 0; i < NUM_TESTS; i++) {
        num = rand();
        /*num = i;*/
        nums[i] = num;
        if (num % 2 == 1) {
            test_nums[test_nums_size++] = num;
        }
    }
    uint64_t* new_end = remove_if(nums, nums + NUM_TESTS, sizeof(uint64_t), (algorithm_predicate_t)is_even);
    test(nums + test_nums_size == new_end);
    test(memcmp(test_nums, nums, test_nums_size * sizeof(uint64_t)) == 0);

    new_end = remove_ifn(nums, new_end,  sizeof(uint64_t), (algorithm_predicate_t)is_even);
    test(nums == new_end);
}

int main(void)
{
    test_remove_if();
    return 0;
}
