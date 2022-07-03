#ifndef BUFFER_H
#define BUFFER_H

#include "../ds_error/ds_error.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
typedef struct buffer_t buffer_t;

ds_error_t buffer_size(buffer_t* b, uint64_t* size);
ds_error_t buffer_end(buffer_t* b, void** end);
ds_error_t buffer_begin(buffer_t* b, void** begin);
ds_error_t buffer_remove(buffer_t* b, uint64_t index);
ds_error_t buffer_more(buffer_t* b, void** ret);
ds_error_t buffer_free(buffer_t* b);
ds_error_t buffer_get(buffer_t* b, uint64_t index, void** ret);
ds_error_t buffer_create(buffer_t** b, uint64_t initial_size, uint64_t member_size);

#endif
