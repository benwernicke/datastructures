#include "../test/test.h"
#include "map.h"

#define NUM_TESTS 10

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
    map_t* map = NULL;
    ds_error_t err = map_create(&map, (map_hash_function_t)uint_hash, (map_cmp_function_t)uint_cmp, 1);
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
        map_get(map, &nums[i], (void**)&value);
        test_bool((char*)__func__, value == NULL);
    }

    for (i = 0; i < NUM_TESTS; i++) {
        err = map_insert(map, &nums[i], &nums[i]);
        if (err != SUCCESS) {
            fprintf(stderr, "error: create %s\n", __func__);
            exit(1);
        }
    }

    for (i = 0; i < NUM_TESTS; i++) {
        if (i % 2 == 0) {
            map_delete(map, &nums[i]);
        }
    }
    for (i = 0; i < NUM_TESTS; i++) {
        map_get(map, &nums[i], (void**)&value);
        if (i % 2 == 0) {
            test_bool((char*)__func__, value == NULL);
        } else {
            test_bool((char*)__func__, value == &nums[i]);
        }
    }
    map_free(map);
}

void test_insert_get(void)
{
    map_t* map = NULL;
    ds_error_t err = map_create(&map, (map_hash_function_t)uint_hash, (map_cmp_function_t)uint_cmp, 1);
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
        map_get(map, &nums[i], (void**)&value);
        test_bool((char*)__func__, value == NULL);
    }

    for (i = 0; i < NUM_TESTS; i++) {
        err = map_insert(map, &nums[i], &nums[i]);
        if (err != SUCCESS) {
            fprintf(stderr, "error: create %s\n", __func__);
            exit(1);
        }
    }

    for (i = 0; i < NUM_TESTS; i++) {
        map_get(map, &nums[i], (void**)&value);
        test_bool((char*)__func__, value != NULL);
        test_bool((char*)__func__, value == &nums[i]);
    }

    uint64_t num_found = 0;
    map_iterator_t iter;
    map_iterator_t end;
    map_iterator_begin(map, &iter);
    map_iterator_end(map, &end);
    for (; iter != end; map_iterator_next(map, &iter)) {
        value = map_iterator_value(map, iter);
        for (i = 0; i < NUM_TESTS; i++) {
            if (nums[i] == *value) {
                num_found++;
                break;
            }
        }
    }
    test_bool((char*)__func__, num_found == NUM_TESTS);

    map_free(map);
}

int main(void)
{
    printf("HASHMAP TEST\n");
    test_insert_get();
    test_total();
    return 0;
}
