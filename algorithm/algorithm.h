#ifndef ALGORITHM_H
#define ALGORITHM_H
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

typedef bool(*algorithm_predicate_t)(void* val);

void* remove_if(void* begin, void* end, uint64_t block_size, algorithm_predicate_t pred);
void* remove_ifn(void* begin, void* end, uint64_t block_size, algorithm_predicate_t pred);
void* find_ifn(void* begin, void* end, uint64_t block_size, algorithm_predicate_t pred);
void* find_if(void* begin, void* end, uint64_t block_size, algorithm_predicate_t pred);

#endif
