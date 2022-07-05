#ifndef HASHSET_H
#define HASHSET_H
#include "../ds_error/ds_error.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct hashset_t hashset_t;
typedef uint64_t (*hashset_hash_function_t)(void* key);
typedef bool (*hashset_cmp_function_t)(void* a, void* b);

ds_error_t hashset_create(hashset_t** hs, hashset_hash_function_t hash, hashset_cmp_function_t cmp, uint64_t initial_size);
ds_error_t hashset_set(hashset_t* hs, void* key);
ds_error_t hashset_has(hashset_t* hs, void* key, bool* has_key);
ds_error_t hashset_begin(hashset_t* hs, void*** begin);
ds_error_t hashset_end(hashset_t* hs, void*** end);
ds_error_t hashset_size(hashset_t* hs, uint64_t* size);
ds_error_t hashset_delete(hashset_t* hs, void* key);
ds_error_t hashset_get_keys(hashset_t* hs, void** buf, uint64_t buf_len);
ds_error_t hashset_free(hashset_t* hs);

#endif
