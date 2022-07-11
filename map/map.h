#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifndef MAP_INT
#define MAP_INT uint64_t
#endif

typedef struct map_t map_t;

typedef MAP_INT (*map_hash_function_t)(void* key);
typedef bool (*map_cmp_function_t)(void* a, void* b);

typedef MAP_INT map_iterator_t;

map_t* map_create(map_cmp_function_t cmp, MAP_INT initial_size);
void map_free(map_t* map);

int map_insert(map_t* map, MAP_INT hash, void* key, void* value);
void* map_get_or_insert(map_t* map, MAP_INT hash, void* key, void* value);
void* map_get(map_t* map, MAP_INT hash, void* key);
void map_delete(map_t* map, MAP_INT hash, void* key);
bool map_contains(map_t* map, MAP_INT hash, void* key);

MAP_INT map_size(map_t* map);

void* map_iterator_key(map_t* map, map_iterator_t iter);
void* map_iterator_value(map_t* map, map_iterator_t iter);
map_iterator_t map_iterator_next(map_t* map, map_iterator_t iter);

#define MAP_ITERATOR_INIT ((map_iterator_t)-1)
#define MAP_ITERATOR_END ((map_iterator_t)-1)
#endif
