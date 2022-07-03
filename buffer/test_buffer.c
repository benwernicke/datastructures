#include "../test/test.h"
#include "buffer.h"

#define NUM_TESTS 10000

void test_buffer_from_range()
{
    buffer_t* b;
    if (buffer_create(&b, 1, sizeof(uint64_t)) != DS_SUCCESS) {
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
        if (buffer_more(b, (void**)&value) != DS_SUCCESS) {
            fprintf(stderr, "error: more: %s\n", __func__);
            exit(1);
        }
        *value = nums[i];
    }

    buffer_t* bra;
    buffer_t* brb;

    if (buffer_create_from_range(&bra, NUM_TESTS, sizeof(uint64_t), nums, sizeof(nums)) != DS_SUCCESS) {
        fprintf(stderr, "error: create: %s\n", __func__);
        exit(1);
    }
    uint64_t* begin = NULL;
    uint64_t* end = NULL;
    buffer_begin(b, (void**)&begin);
    buffer_end(b, (void**)&end);

    if (buffer_create_from_range(&brb, NUM_TESTS, sizeof(uint64_t), begin, NUM_TESTS * sizeof(uint64_t)) != DS_SUCCESS) {
        fprintf(stderr, "error: create: %s\n", __func__);
        exit(1);
    }

    uint64_t sizeb = 0;
    uint64_t sizebra = 0;
    uint64_t sizebrb = 0;

    buffer_size(b, &sizeb);
    buffer_size(b, &sizebra);
    buffer_size(b, &sizebrb);
    test_bool((char*)__func__, sizeb == NUM_TESTS);
    test_bool((char*)__func__, sizebra == NUM_TESTS);
    test_bool((char*)__func__, sizebrb == NUM_TESTS);

    buffer_begin(b, (void**)&begin);
    test_bool((char*)__func__, memcmp(begin, nums, sizeof(nums)) == 0);

    buffer_begin(bra, (void**)&begin);
    test_bool((char*)__func__, memcmp(begin, nums, sizeof(nums)) == 0);

    buffer_begin(brb, (void**)&begin);
    test_bool((char*)__func__, memcmp(begin, nums, sizeof(nums)) == 0);

    buffer_free(b);
    buffer_free(bra);
    buffer_free(brb);
}

void test_more_get()
{
    buffer_t* b;
    if (buffer_create(&b, 1, sizeof(uint64_t)) != DS_SUCCESS) {
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
        if (buffer_more(b, (void**)&value) != DS_SUCCESS) {
            fprintf(stderr, "error: more: %s\n", __func__);
            exit(1);
        }
        *value = nums[i];
    }
    uint64_t size;
    buffer_size(b, &size);
    test_bool((char*)__func__, size == (uint64_t)NUM_TESTS);
    uint64_t* begin = NULL;
    buffer_begin(b, (void**)&begin);

    test_bool((char*)__func__, memcmp(begin, nums, sizeof(nums)) == 0);

    for (i = 0; i < NUM_TESTS; i++) {
        if (buffer_get(b, i, (void**)&value) != DS_SUCCESS) {
            fprintf(stderr, "error: more: %s\n", __func__);
            exit(1);
        }
        test_bool((char*)__func__, *value == nums[i]);
    }

    buffer_free(b);
}

void test_size_remove_ptr()
{
    buffer_t* b;
    if (buffer_create(&b, 1, sizeof(uint64_t)) != DS_SUCCESS) {
        fprintf(stderr, "error: create: %s\n", __func__);
        exit(1);
    }
    uint64_t i;
    uint64_t nums[NUM_TESTS];
    for (i = 0; i < NUM_TESTS; i++) {
        nums[i] = i;
    }

    uint64_t* value = NULL;
    for (i = 0; i < NUM_TESTS; i++) {
        if (buffer_more(b, (void**)&value) != DS_SUCCESS) {
            fprintf(stderr, "error: more: %s\n", __func__);
            exit(1);
        }
        *value = nums[i];
    }

    uint64_t size;
    buffer_size(b, &size);
    test_bool((char*)__func__, size == (uint64_t)NUM_TESTS);
    uint64_t* begin = NULL;
    buffer_begin(b, (void**)&begin);

    test_bool((char*)__func__, memcmp(begin, nums, sizeof(nums)) == 0);
    if (buffer_index_from_ptr(b, begin, &i) != DS_SUCCESS) {
        fprintf(stderr, "error: index_from_ptr: %s\n", __func__);
        exit(1);
    }
    test_bool((char*)__func__, i == 0);

    uint64_t numsa[NUM_TESTS - 1];
    for (i = 0; i < NUM_TESTS - 1; i++) {
        numsa[i] = i;
    }
    buffer_remove(b, size - 1);
    buffer_size(b, &size);
    buffer_begin(b, (void**)&begin);
    test_bool((char*)__func__, size == NUM_TESTS - 1);
    test_bool((char*)__func__, memcmp(begin, numsa, sizeof(numsa)) == 0);

    uint64_t numsb[NUM_TESTS - 2];
    for (i = 1; i < NUM_TESTS - 1; i++) {
        numsb[i - 1] = i;
    }

    buffer_remove(b, 0);
    buffer_size(b, &size);
    buffer_begin(b, (void**)&begin);
    test_bool((char*)__func__, size == NUM_TESTS - 2);
    test_bool((char*)__func__, memcmp(begin, numsb, sizeof(numsb)) == 0);

    buffer_free(b);
}

void test_size_remove()
{
    buffer_t* b;
    if (buffer_create(&b, 1, sizeof(uint64_t)) != DS_SUCCESS) {
        fprintf(stderr, "error: create: %s\n", __func__);
        exit(1);
    }
    uint64_t i;
    uint64_t nums[NUM_TESTS];
    for (i = 0; i < NUM_TESTS; i++) {
        nums[i] = i;
    }

    uint64_t* value = NULL;
    for (i = 0; i < NUM_TESTS; i++) {
        if (buffer_more(b, (void**)&value) != DS_SUCCESS) {
            fprintf(stderr, "error: more: %s\n", __func__);
            exit(1);
        }
        *value = nums[i];
    }

    uint64_t size;
    buffer_size(b, &size);
    test_bool((char*)__func__, size == (uint64_t)NUM_TESTS);
    uint64_t* begin = NULL;
    buffer_begin(b, (void**)&begin);

    test_bool((char*)__func__, memcmp(begin, nums, sizeof(nums)) == 0);

    uint64_t numsa[NUM_TESTS - 1];
    for (i = 0; i < NUM_TESTS - 1; i++) {
        numsa[i] = i;
    }
    buffer_begin(b, (void**)&begin);
    begin += NUM_TESTS - 1;

    buffer_remove_ptr(b, begin);
    buffer_size(b, &size);
    buffer_begin(b, (void**)&begin);
    test_bool((char*)__func__, size == NUM_TESTS - 1);
    test_bool((char*)__func__, memcmp(begin, numsa, sizeof(numsa)) == 0);

    uint64_t numsb[NUM_TESTS - 2];
    for (i = 1; i < NUM_TESTS - 1; i++) {
        numsb[i - 1] = i;
    }

    buffer_begin(b, (void**)&begin);
    buffer_remove_ptr(b, begin);
    buffer_size(b, &size);
    buffer_begin(b, (void**)&begin);
    test_bool((char*)__func__, size == NUM_TESTS - 2);
    test_bool((char*)__func__, memcmp(begin, numsb, sizeof(numsb)) == 0);

    buffer_free(b);
}

int main(void)
{
    printf("BUFFER TEST\n");
    test_more_get();
    test_size_remove();
    test_size_remove_ptr();
    test_buffer_from_range();
    test_total();
    return 0;
}
