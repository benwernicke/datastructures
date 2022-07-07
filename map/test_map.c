#include "../test/test.h"
#include "map.h"

#define NUM_TESTS 10000

void test_delete(void)
{
    map_t* map = map_create(map_b64_self, map_u64_cmp, 1);
    if (map == NULL) {
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
        value = map_get(map, &nums[i]);
        test(value == NULL);
    }

    int err;
    for (i = 0; i < NUM_TESTS; i++) {
        err = map_insert(map, &nums[i], &nums[i]);
        if (err != 0) {
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
        value = map_get(map, &nums[i]);
        if (i % 2 == 0) {
            test(value == NULL);
        } else {
            test(value == &nums[i]);
        }
    }
    map_free(map);
}

void test_insert_get(void)
{
    map_t* map = map_create(map_b64_self, map_u64_cmp, 1);
    if (map == NULL) {
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
        value = map_get(map, &nums[i]);
        test(value == NULL);
    }

    int err;
    for (i = 0; i < NUM_TESTS; i++) {
        err = map_insert(map, &nums[i], &nums[i]);
        if (err != 0) {
            fprintf(stderr, "error: create %s\n", __func__);
            exit(1);
        }
    }

    test(map_size(map) == NUM_TESTS);
    

    for (i = 0; i < NUM_TESTS; i++) {
        test(map_contains(map, &nums[i]));
        value = map_get(map, &nums[i]);
        test( value != NULL);
        test(value == &nums[i]);
    }

    uint64_t num_found = 0;

    map_iterator_t iter = MAP_ITERATOR_INIT;
    while ((iter = map_iterator_next(map, iter)) != MAP_ITERATOR_END) {
        value = map_iterator_value(map, iter);
        for (i = 0; i < NUM_TESTS; i++) {
            if (nums[i] == *value) {
                num_found++;
                break;
            }
        }
    }

    test(num_found == NUM_TESTS);

    map_free(map);
}

int main(void)
{
    printf("HASHMAP TEST\n");
    test_insert_get();
    return 0;
}
