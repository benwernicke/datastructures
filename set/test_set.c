#include "../test/test.h"
#include "set.h"

#define NUM_TESTS 10

bool u64_cmp(void* a, void* b)
{
    return *(uint64_t*)a == *(uint64_t*)b;
}

void test_delete(void)
{
    set_t* set = set_create(u64_cmp, 10);
    uint64_t nums[NUM_TESTS];
    uint64_t i;
    for (i = 0; i < NUM_TESTS; i++) {
        nums[i] = i;
        if (set_insert(set, nums[i], &nums[i]) != 0) {
            fprintf(stderr, "something went wrong in %s\n", __func__);
        }
    }
    for (i = 0; i < NUM_TESTS; i++) {
        if (i % 2 == 0) {
            set_delete(set, nums[i], &nums[i]);
        }
    }

    bool b;
    for (i = 0; i < NUM_TESTS; i++) {
        b = set_contains(set, nums[i], &nums[i]);
        if (i % 2 == 0) {
            test(!b);
        } else {
            test(b);
        }
    }
    set_free(set);
}

void test_get_set()
{
    set_t* set = set_create(u64_cmp, 10);
    srand(69);
    uint64_t i;
    uint64_t nums[NUM_TESTS];
    for (i = 0; i < NUM_TESTS; i++) {
        nums[i] = rand();
    }
    for (i = 0; i < NUM_TESTS; i++) {
        if (set_insert(set, nums[i], &nums[i]) != 0) {
            fprintf(stderr, "something went wrong in %s\n", __func__);
        }
    }
    bool b;
    for (i = 0; i < NUM_TESTS; i++) {
        b = set_contains(set, nums[i], &nums[i]);
        test( b);
    }
    set_free(set);
}

void test_get_keys_size()
{
    set_t* set = set_create(u64_cmp, 10);
    srand(69);
    uint64_t i;
    uint64_t nums[NUM_TESTS];
    for (i = 0; i < NUM_TESTS; i++) {
        nums[i] = rand();
    }
    for (i = 0; i < NUM_TESTS; i++) {
        if (set_insert(set, nums[i], &nums[i]) != 0) {
            fprintf(stderr, "something went wrong in %s\n", __func__);
        }
    }
    uint64_t size = set_size(set);

    test(size == NUM_TESTS);

    set_iterator_t iter = SET_ITERATOR_INIT;
    uint64_t found = 0;
    while ((iter = set_iterator_next(set, iter)) != SET_ITERATOR_END) {
        for (i = 0; i < NUM_TESTS; i++) {
            if (*(uint64_t*)set_iterator_key(set, iter) == nums[i]) {
                found++;
                break;
            }
        }
    }
    test(found == NUM_TESTS);

    set_free(set);
}
int main(void)
{
    printf("HASHSET TESTS\n");
    test_get_set();
    test_delete();
    test_get_keys_size();
    return 0;
}
