#include "../test/test.h"
#include "hashmap.h"

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

void test_insert_get(void)
{
    hashmap_t* map = NULL;
    ds_error_t err = hashmap_create(&map, (hashmap_hash_function_t)uint_hash, (hashmap_cmp_function_t)uint_cmp, 1);
    if (err != SUCCESS) {
        fprintf(stderr, "error: create %s\n", __func__);
        exit(1);
    }

    uint64_t nums[NUM_TESTS];
    uint64_t i;
    for (i = 0; i < NUM_TESTS; i++) {
        nums[i] = i;
    }

    uint64_t* value;

    for (i = 0; i < NUM_TESTS; i++) {
        hashmap_get(map, &nums[i], (void**)&value);
        test_bool((char*)__func__, value == NULL);
    }

    for (i = 0; i < NUM_TESTS; i++) {
        err = hashmap_insert(map, &nums[i], &nums[i]);
        if (err != SUCCESS) {
            fprintf(stderr, "error: create %s\n", __func__);
            exit(1);
        }
    }

    for (i = 0; i < NUM_TESTS; i++) {
        hashmap_get(map, &nums[i], (void**)&value);
        test_bool((char*)__func__, value != NULL);
        test_bool((char*)__func__, value == &nums[i]);
    }

    hashmap_free(map);
}

int main(void)
{
    printf("HASHMAP TEST\n");
    test_insert_get();
    return 0;
}
