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

void* map_check_and_insert(map_t* map, void* key, void* value, MAP_INT* pos);

void* map_get(map_t* map, void* key);
void map_free(map_t* map);

map_t* map_create(map_hash_function_t hash, map_cmp_function_t cmp, MAP_INT initial_size);
int map_insert(map_t* map, void* key, void* value);

void map_delete(map_t* map, void* key);
MAP_INT map_size(map_t* map);

void* map_iterator_key(map_t* map, map_iterator_t iter);
void* map_iterator_value(map_t* map, map_iterator_t iter);
map_iterator_t map_iterator_next(map_t* map, map_iterator_t iter);
bool map_contains(map_t* map, void* key);

#define MAP_ITERATOR_INIT ((map_iterator_t)-1)
#define MAP_ITERATOR_END ((map_iterator_t)-1)

// some hashfunctions ----------------------------------------------------------------

// char* hashfunctions null terminated
MAP_INT map_simple_str_hash(void* key);
MAP_INT map_str_djb2(void* key);
MAP_INT map_str_jenkins(void* key);

// char* cmp function
bool map_str_cmp(void* a, void* b);

// int cmp functions
bool map_i64_cmp(void* a, void* b);
bool map_i32_cmp(void* a, void* b);
bool map_i16_cmp(void* a, void* b);
bool map_i8_cmp(void* a, void* b);

// uint cmp functions
bool map_u64_cmp(void* a, void* b);
bool map_u32_cmp(void* a, void* b);
bool map_u16_cmp(void* a, void* b);
bool map_u8_cmp(void* a, void* b);

// interprets block as uints and returns it probably best for hashing ints
MAP_INT map_b64_self(void* key);
MAP_INT map_b32_self(void* key);
MAP_INT map_b16_self(void* key);
MAP_INT map_b8_self(void* key);

#endif
