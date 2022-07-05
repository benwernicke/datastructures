#ifndef HASHSET_H
#define HASHSET_H
#include "../ds_error/ds_error.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct set_t set_t;
typedef uint64_t (*set_hash_function_t)(void* key);
typedef bool (*set_cmp_function_t)(void* a, void* b);

ds_error_t set_create(set_t** hs, set_hash_function_t hash, set_cmp_function_t cmp, uint64_t initial_size);
ds_error_t set_set(set_t* hs, void* key);
ds_error_t set_has(set_t* hs, void* key, bool* has_key);
ds_error_t set_begin(set_t* hs, void*** begin);
ds_error_t set_end(set_t* hs, void*** end);
ds_error_t set_size(set_t* hs, uint64_t* size);
ds_error_t set_delete(set_t* hs, void* key);
ds_error_t set_get_keys(set_t* hs, void** buf, uint64_t buf_len);
ds_error_t set_free(set_t* hs);

#endif
