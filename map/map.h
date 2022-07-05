#ifndef HASHMAP_H
#define HASHMAP_H

#include "../ds_error/ds_error.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct map_t map_t;

typedef uint64_t (*map_hash_function_t)(void* key);
typedef bool (*map_cmp_function_t)(void* a, void* b);

typedef uintptr_t map_iterator_t;

ds_error_t map_get(map_t* map, void* key, void** value);
ds_error_t map_insert(map_t* map, void* key, void* value);
ds_error_t map_free(map_t* map);
ds_error_t map_create(map_t** map, map_hash_function_t hash, map_cmp_function_t cmp, uint64_t initial_size);
ds_error_t map_delete(map_t* map, void* key);

ds_error_t map_iterator_begin(map_t* map, map_iterator_t* iter);
ds_error_t map_iterator_end(map_t* map, map_iterator_t* end);
void* map_iterator_key(map_t* map, map_iterator_t iter);
void* map_iterator_value(map_t* map, map_iterator_t iter);
ds_error_t map_iterator_next(map_t* map, map_iterator_t* iter);

// some hashfunctions

uint64_t map_simple_str_hash(void* key);
bool map_str_cmp(void* a, void* b);

#endif
