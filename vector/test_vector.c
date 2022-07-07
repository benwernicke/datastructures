#include "../test/test.h"
#include "vector.h"

#define NUM_TESTS 10

void test_remove(void)
{
    vector_t* v = vector_create(1);
    uint64_t nums[NUM_TESTS];

    uint64_t** more;

    uint64_t i;
    for (i = 0; i < NUM_TESTS; i++) {
        nums[i] = i;
        more = (uint64_t**)vector_more(v);
        *more = &nums[i];
    }

    uint64_t* nums_iter = nums;
    uint64_t** v_iter = (uint64_t**)vector_begin(v);
    uint64_t** v_end = (uint64_t**)vector_end(v);

    while (v_iter != v_end && nums_iter != nums + NUM_TESTS) {
        test(*v_iter == nums_iter);
        v_iter++;
        nums_iter++;
    }

    vector_remove(v, 0);
    vector_remove(v, NUM_TESTS - 1);

    uint64_t size = vector_size(v);
    test(size == NUM_TESTS - 2);

    nums_iter = nums + 1;
    v_iter = (uint64_t**)vector_begin(v);
    v_end = (uint64_t**)vector_end(v);

    while (v_iter != v_end && nums_iter != nums + NUM_TESTS - 2) {
        test(*v_iter == nums_iter);
        v_iter++;
        nums_iter++;
    }

    vector_free(v);
}

void test_more()
{
    vector_t* v = vector_create(1);
    if (v == NULL) {
        fprintf(stderr, "error: something create: %s\n", __func__);
        exit(1);
    }

    uint64_t i;
    uint64_t nums[NUM_TESTS];
    uint64_t** vmore;

    for (i = 0; i < NUM_TESTS; i++) {
        nums[i] = i;
        vmore = (uint64_t**)vector_more(v);
        if (vmore == NULL) {
            fprintf(stderr, "error: something more: %s\n", __func__);
            exit(1);
        }
        *vmore = &nums[i];
    }

    uint64_t** iter = (uint64_t**)vector_begin(v);
    uint64_t** end = (uint64_t**)vector_end(v);

    uint64_t* nums_iter = nums;

    for (; iter != end; iter++, nums_iter++) {
        test(*iter == nums_iter);
    }

    uint64_t size = vector_size(v);
    test(size == NUM_TESTS);
    vector_free(v);
}

int main(void)
{
    printf("VECTOR TESTS\n");
    test_more();
    test_remove();
    return 0;
}
