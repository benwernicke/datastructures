#include "../test/test.h"
#include "vector.h"

#define NUM_TESTS 10
void test_more_remove()
{
    vector_t* v = NULL;
    if (vector_create(&v, 1) != SUCCESS) {
        fprintf(stderr, "error: something create: %s\n", __func__);
        exit(1);
    }

    uint64_t i;
    uint64_t nums[NUM_TESTS];
    uint64_t** vmore;

    for (i = 0; i < NUM_TESTS; i++) {
        nums[i] = i;
        if (vector_more(v, (void***)&vmore) != SUCCESS) {
            fprintf(stderr, "error: something more: %s\n", __func__);
            exit(1);
        }
        *vmore = &nums[i];
    }

    uint64_t** iter;
    uint64_t** end;
    vector_begin(v, (void***)&iter);
    vector_end(v, (void***)&end);

    uint64_t* nums_iter = nums;

    for (; iter != end; iter++, nums_iter++) {
        test_bool((char*)__func__, *iter == nums_iter);
    }

    uint64_t size = 0;
    vector_size(v, &size);
    test_bool((char*)__func__, size == NUM_TESTS);

    vector_remove(v, size - 1);
    vector_remove(v, 0);

    vector_size(v, &size);
    test_bool((char*)__func__, size == NUM_TESTS - 2);

    vector_begin(v, (void***)&iter);
    vector_end(v, (void***)&end);
    nums_iter = nums + 1;
    for (; iter != end; iter++, nums_iter++) {
        test_bool((char*)__func__, *iter == nums_iter);
    }

    vector_free(v);
}

int main(void)
{
    printf("VECTOR TESTS\n");
    test_more_remove();
    test_total();
    return 0;
}
