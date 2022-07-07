#include "algorithm.h"

static inline void* next_(void* iter, uint64_t block_size)
{
    return (uint8_t*)iter + block_size;
}

void* find_if(void* begin, void* end, uint64_t block_size, algorithm_predicate_t pred)
{
    for (; begin != end; begin = next_(begin, block_size)) {
        if (pred(begin)) {
            return begin;
        }
    }
    return begin;
}
void* find_ifn(void* begin, void* end, uint64_t block_size, algorithm_predicate_t pred)
{
    for (; begin != end; begin = next_(begin, block_size)) {
        if (!pred(begin)) {
            return begin;
        }
    }
    return begin;
}

static inline uint64_t move_(uint8_t* dest, uint8_t* src, uint64_t n, uint8_t* end)
{
    if (src + n >= end) {
        memmove(dest, src, end - src);
        return end - src;
    } else {
        memmove(dest, src, n);
        return n;
    }
}

void* remove_if(void* begin, void* end, uint64_t block_size, algorithm_predicate_t pred)
{
    uint64_t orig_len = (uint8_t*)end - (uint8_t*)begin;
    uint64_t moved = 0;
    uint8_t* iter = find_if(begin, end, block_size, pred);
    uint8_t* last = iter;
    uint8_t* middle;
    while (iter != end) {
        iter = find_ifn(iter, end, block_size, pred);
        middle = iter;
        iter = find_if(iter, end, block_size, pred);
        memmove(last, middle, iter - middle);
        last += (iter - middle);
    }
    return last;
}

void* remove_ifn(void* begin, void* end, uint64_t block_size, algorithm_predicate_t pred)
{
    uint64_t orig_len = (uint8_t*)end - (uint8_t*)begin;
    uint64_t moved = 0;
    uint8_t* iter = find_ifn(begin, end, block_size, pred);
    uint8_t* last = iter;
    uint8_t* middle;
    while (iter != end) {
        iter = find_if(iter, end, block_size, pred);
        middle = iter;
        iter = find_ifn(iter, end, block_size, pred);
        memmove(last, middle, iter - middle);
        last += (iter - middle);
    }
    return last;
}
