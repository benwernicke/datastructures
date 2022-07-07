#include "buffer.h"
#include <assert.h>
#include <stdio.h>

#include "../test/test.h"

#define NUM_TESTS 10

void test_buffer_from_range()
{
    buffer_t* b = buffer_create(1, sizeof(uint64_t));
    if (b == NULL) {
        fprintf(stderr, "error: create: %s\n", __func__);
        exit(1);
    }

    srand(69);
    uint64_t i;
    uint64_t nums[NUM_TESTS];
    for (i = 0; i < NUM_TESTS; i++) {
        nums[i] = rand();
    }

    uint64_t* value = NULL;
    for (i = 0; i < NUM_TESTS; i++) {
        value = buffer_more(b);
        if (value == NULL) {
            fprintf(stderr, "error: more: %s\n", __func__);
            exit(1);
        }
        *value = nums[i];
    }

    buffer_t* bra = buffer_create_from_range(NUM_TESTS, sizeof(uint64_t), nums, sizeof(nums));
    uint64_t* begin = buffer_begin(b);
    buffer_t* brb = buffer_create_from_range(NUM_TESTS, sizeof(uint64_t), begin, sizeof(nums));

    if (bra == NULL) {
        fprintf(stderr, "error: create: %s\n", __func__);
        exit(1);
    }

    if (brb == NULL) {
        fprintf(stderr, "error: create: %s\n", __func__);
        exit(1);
    }

    uint64_t sizeb = buffer_size(b);
    uint64_t sizebra = buffer_size(bra);
    uint64_t sizebrb = buffer_size(brb);

    test(sizeb == NUM_TESTS);
    test(sizebra == NUM_TESTS);
    test(sizebrb == NUM_TESTS);

    begin = buffer_begin(b);
    test(memcmp(begin, nums, sizeof(nums)) == 0);

    begin = buffer_begin(bra);
    test(memcmp(begin, nums, sizeof(nums)) == 0);

    begin = buffer_begin(brb);
    test(memcmp(begin, nums, sizeof(nums)) == 0);

    buffer_free(b);
    buffer_free(bra);
    buffer_free(brb);
}

void test_more_get()
{
    buffer_t* b = buffer_create(1, sizeof(uint64_t));
    if (b == NULL) {
        fprintf(stderr, "error: create: %s\n", __func__);
        exit(1);
    }

    srand(69);
    uint64_t i;
    uint64_t nums[NUM_TESTS];
    for (i = 0; i < NUM_TESTS; i++) {
        nums[i] = rand();
    }

    uint64_t* value = NULL;
    for (i = 0; i < NUM_TESTS; i++) {
        value = buffer_more(b);
        if (value == NULL) {
            fprintf(stderr, "error: more: %s\n", __func__);
            exit(1);
        }
        *value = nums[i];
    }
    uint64_t size = buffer_size(b);
    test(size == NUM_TESTS);

    uint64_t* begin = buffer_begin(b);
    test(memcmp(begin, nums, sizeof(nums)) == 0);

    for (i = 0; i < NUM_TESTS; i++) {
        value = buffer_get(b, i);
        test(*value == nums[i]);
    }
    buffer_free(b);
}

void test_buffer_remove(void)
{
    buffer_t* b = buffer_create(1, sizeof(uint64_t));
    uint64_t nums[NUM_TESTS];
    uint64_t* more;

    uint64_t i;
    for (i = 0; i < NUM_TESTS; i++) {
        nums[i] = i;
        more = buffer_more(b);
        *more = nums[i];
    }

    uint64_t* iter = buffer_begin(b);
    test(memcmp(iter, nums, sizeof(nums)) == 0);

    buffer_remove(b, 0);
    buffer_remove(b, NUM_TESTS - 1);
    uint64_t size = buffer_size(b);

    test(size == NUM_TESTS - 2);

    iter = buffer_begin(b);
    test(memcmp(iter, nums + 1, size * sizeof(uint64_t)) == 0);
    buffer_free(b);
}

int main(void)
{
    printf("BUFFER TEST\n");
    test_more_get();
    test_buffer_from_range();
    test_buffer_remove();
    return 0;
}
