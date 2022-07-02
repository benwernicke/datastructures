#include "../test/test.h"
#include "hashset.h"

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
    hashset_t* hs;
    hashset_create(&hs, (hashset_hash_function_t)uint_hash, (hashset_cmp_function_t)uint_cmp, 10);
    uint64_t nums[NUM_TESTS];
    uint64_t i;
    for (i = 0; i < NUM_TESTS; i++) {
        nums[i] = i;
        if (hashset_set(hs, &nums[i]) != DS_SUCCESS) {
            fprintf(stderr, "something went wrong in %s\n", __func__);
        }
    }
    for (i = 0; i < NUM_TESTS; i++) {
        if (i % 2 == 0) {
            hashset_delete(hs, &nums[i]);
        }
    }

    bool b;
    for (i = 0; i < NUM_TESTS; i++) {
        if (hashset_has(hs, &nums[i], &b) != DS_SUCCESS) {
            fprintf(stderr, "something went wrong in %s\n", __func__);
        }
        if (i % 2 == 0) {
            test_bool((char*)__func__, !b);
        } else {
            test_bool((char*)__func__, b);
        }
    }
    hashset_free(hs);
}

void test_get_set()
{
    hashset_t* hs;
    hashset_create(&hs, (hashset_hash_function_t)uint_hash, (hashset_cmp_function_t)uint_cmp, 10);
    srand(69);
    uint64_t i;
    uint64_t nums[NUM_TESTS];
    for (i = 0; i < NUM_TESTS; i++) {
        nums[i] = rand();
    }
    for (i = 0; i < NUM_TESTS; i++) {
        if (hashset_set(hs, &nums[i]) != DS_SUCCESS) {
            fprintf(stderr, "something went wrong in %s\n", __func__);
        }
    }
    bool b;
    for (i = 0; i < NUM_TESTS; i++) {
        if (hashset_has(hs, &nums[i], &b) != DS_SUCCESS) {
            fprintf(stderr, "something went wrong in %s\n", __func__);
        }
        test_bool((char*)__func__, b);
    }
    hashset_free(hs);
}

void test_get_keys_size()
{
    hashset_t* hs;
    hashset_create(&hs, (hashset_hash_function_t)uint_hash, (hashset_cmp_function_t)uint_cmp, 10);
    srand(69);
    uint64_t i;
    uint64_t nums[NUM_TESTS];
    for (i = 0; i < NUM_TESTS; i++) {
        nums[i] = rand();
    }
    for (i = 0; i < NUM_TESTS; i++) {
        if (hashset_set(hs, &nums[i]) != DS_SUCCESS) {
            fprintf(stderr, "something went wrong in %s\n", __func__);
        }
    }
    uint64_t size;
    hashset_size(hs, &size);
    uint64_t* keys[size];
    uint64_t* real_keys[size];

    test_bool((char*)__func__, size == NUM_TESTS);
    for (i = 0; i < NUM_TESTS; i++) {
        real_keys[i] = &nums[i];
    }
    hashset_get_keys(hs, (void**)keys, size);
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
