#ifndef BUFFER_H
#define BUFFER_H

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
typedef struct buffer_t buffer_t;

uint64_t buffer_size(buffer_t* b);
void* buffer_end(buffer_t* b);
void* buffer_begin(buffer_t* b);

void* buffer_more(buffer_t* b);
void buffer_free(buffer_t* b);
void* buffer_get(buffer_t* b, uint64_t index);
buffer_t* buffer_create(uint64_t initial_size, uint64_t member_size);
uint64_t buffer_index_from_ptr(buffer_t* b, void* ptr);
buffer_t* buffer_create_from_range(uint64_t initial_size, uint64_t member_size, void* range, uint64_t range_size);
void buffer_remove(buffer_t* b, uint64_t index);

#endif
