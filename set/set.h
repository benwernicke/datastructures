#ifndef SET_H
#define SET_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifndef SET_INT
#define SET_INT uint64_t
#endif

typedef struct set_t set_t;

typedef SET_INT (*set_hash_function_t)(void* key);
typedef bool (*set_cmp_function_t)(void* a, void* b);

typedef uintptr_t set_iterator_t;


bool set_contains(set_t* set, void* key);
void set_free(set_t* set);

set_t* set_create(set_hash_function_t hash, set_cmp_function_t cmp, SET_INT initial_size);
int set_insert(set_t* set, void* key);

void set_delete(set_t* set, void* key);
SET_INT set_size(set_t* set);

void* set_iterator_value(set_t* set, set_iterator_t iter);
set_iterator_t set_iterator_next(set_t* set, set_iterator_t iter);
void* set_iterator_key(set_t* set, set_iterator_t iter);

#define SET_ITERATOR_INIT ((set_iterator_t)-1)
#define SET_ITERATOR_END ((set_iterator_t)-1)

// some hashfunctions ----------------------------------------------------------------

bool set_ptr_cmp(void* a, void* b);
SET_INT set_ptr_self(void* key);

//char* hashfunctions null terminated
SET_INT set_simple_str_hash(void* key);
SET_INT set_str_djb2(void* key);
SET_INT set_str_jenkins(void* key);

//char* cmp function
bool set_str_cmp(void* a, void* b);

//int cmp functions
bool set_i64_cmp(void* a, void* b);
bool set_i32_cmp(void* a, void* b);
bool set_i16_cmp(void* a, void* b);
bool set_i8_cmp(void* a, void* b);

//uint cmp functions
bool set_u64_cmp(void* a, void* b);
bool set_u32_cmp(void* a, void* b);
bool set_u16_cmp(void* a, void* b);
bool set_u8_cmp(void* a, void* b);

//interprets block as uints and returns it probably best for hashing ints
SET_INT set_b64_self(void* key);
SET_INT set_b32_self(void* key);
SET_INT set_b16_self(void* key);
SET_INT set_b8_self(void* key);

#endif
