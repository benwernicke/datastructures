#include "../test/test.h"
#include "set.h"

#define NUM_TESTS 10000

bool uint_cmp(uint64_t* a, uint64_t* b)
{
    if (a == NULL || b == NULL) {
        return 0;
    }
    return *a == *b;
}

uint64_t uint_hash(uint64_t* key)
{
    return *key;
}

void test_delete(void)
{
    set_t* hs;
    set_create(&hs, (set_hash_function_t)uint_hash, (set_cmp_function_t)uint_cmp, 10);
    uint64_t nums[NUM_TESTS];
    uint64_t i;
    for (i = 0; i < NUM_TESTS; i++) {
        nums[i] = i;
        if (set_set(hs, &nums[i]) != SUCCESS) {
            fprintf(stderr, "something went wrong in %s\n", __func__);
        }
    }
    for (i = 0; i < NUM_TESTS; i++) {
        if (i % 2 == 0) {
            set_delete(hs, &nums[i]);
        }
    }

    bool b;
    for (i = 0; i < NUM_TESTS; i++) {
        if (set_has(hs, &nums[i], &b) != SUCCESS) {
            fprintf(stderr, "something went wrong in %s\n", __func__);
        }
        if (i % 2 == 0) {
            test_bool((char*)__func__, !b);
        } else {
            test_bool((char*)__func__, b);
        }
    }
    set_free(hs);
}

void test_get_set()
{
    set_t* hs;
    set_create(&hs, (set_hash_function_t)uint_hash, (set_cmp_function_t)uint_cmp, 10);
    srand(69);
    uint64_t i;
    uint64_t nums[NUM_TESTS];
    for (i = 0; i < NUM_TESTS; i++) {
        nums[i] = rand();
    }
    for (i = 0; i < NUM_TESTS; i++) {
        if (set_set(hs, &nums[i]) != SUCCESS) {
            fprintf(stderr, "something went wrong in %s\n", __func__);
        }
    }
    bool b;
    for (i = 0; i < NUM_TESTS; i++) {
        if (set_has(hs, &nums[i], &b) != SUCCESS) {
            fprintf(stderr, "something went wrong in %s\n", __func__);
        }
        test_bool((char*)__func__, b);
    }
    set_free(hs);
}

void test_get_keys_size()
{
    set_t* hs;
    set_create(&hs, (set_hash_function_t)uint_hash, (set_cmp_function_t)uint_cmp, 10);
    srand(69);
    uint64_t i;
    uint64_t nums[NUM_TESTS];
    for (i = 0; i < NUM_TESTS; i++) {
        nums[i] = rand();
    }
    for (i = 0; i < NUM_TESTS; i++) {
        if (set_set(hs, &nums[i]) != SUCCESS) {
            fprintf(stderr, "something went wrong in %s\n", __func__);
        }
    }
    uint64_t size;
    set_size(hs, &size);
    uint64_t* keys[size];
    uint64_t* real_keys[size];

    test_bool((char*)__func__, size == NUM_TESTS);
    for (i = 0; i < NUM_TESTS; i++) {
        real_keys[i] = &nums[i];
    }
    set_get_keys(hs, (void**)keys, size);
    void** ptr_keys;
    void** ptr_real_keys;
    for (ptr_keys = (void**)keys; ptr_keys != (void**)(keys + size); ptr_keys++) {
        for (ptr_real_keys = (void**)real_keys; ptr_real_keys != (void**)(real_keys + NUM_TESTS); ptr_real_keys++) {
            if (*ptr_keys == *ptr_real_keys) {
                break;
            }
        }
        test_bool((char*)__func__, *ptr_real_keys == *ptr_keys);
    }
    set_free(hs);
}
int main(void)
{
    printf("HASHSET TESTS\n");
    test_get_set();
    test_delete();
    test_get_keys_size();
    test_total();
    return 0;
}
