#include "buffer.h"

struct buffer_t{
    uint64_t allocated;
    uint64_t used;
    uint64_t memsize;
    uint8_t buf;
};
