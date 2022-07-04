#ifndef HASHMAP_H
#define HASHMAP_H

#include "../ds_error/ds_error.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct hashmap_t hashmap_t;

typedef uint64_t (*hashmap_hash_function_t)(void* key);
typedef bool (*hashmap_cmp_function_t)(void* a, void* b);

ds_error_t hashmap_get(hashmap_t* map, void* key, void** value);
ds_error_t hashmap_insert(hashmap_t* map, void* key, void* value);
ds_error_t hashmap_free(hashmap_t* map);
ds_error_t hashmap_create(hashmap_t** map, hashmap_hash_function_t hash, hashmap_cmp_function_t cmp, uint64_t initial_size);
ds_error_t hashmap_delete(hashmap_t* map, void* key);

#endif
