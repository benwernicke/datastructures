#ifndef VSMAP_H
#define VSMAP_H

#include "../ds_error/ds_error.h"
#include "../map/map.h"
#include "../buffer/buffer.h"

typedef struct vsmap_t vsmap_t;

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef uint64_t (*vsmap_hash_function_t)(void* key);
typedef bool (*vsmap_cmp_function_t)(void* a, void* b);

ds_error_t vsmap_get(vsmap_t* map, void* key, void** value);
ds_error_t vsmap_insert(vsmap_t* map, void* key, void** value);
ds_error_t vsmap_free(vsmap_t* map);
ds_error_t vsmap_create(vsmap_t** map, map_hash_function_t hash, map_cmp_function_t cmp, uint64_t initial_size, uint64_t value_size);
ds_error_t vsmap_delete(vsmap_t* map, void* key);

#endif
