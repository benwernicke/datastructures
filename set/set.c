#include "set.h"

static inline int check_realloc_(set_t* set);
static inline uint64_t set_position_(set_t* set, void* key, uint64_t* buf_position, uint64_t* chain);

static inline uint64_t stack_pop_(set_t* set, uint64_t* stack);
static inline void stack_push_buf_(set_t* set, uint64_t* stack, uint64_t iter, uint64_t size);
static inline void stack_push_(set_t* set, uint64_t* stack, uint64_t i);
static inline uint64_t set_size_(set_t* set);

typedef struct entry_t entry_t;
struct entry_t {
    void* key;
    uint64_t next;
    uint64_t prev;
    uint64_t chain;
};

struct set_t {
    uint64_t buf_allocated;
    entry_t* buf;
    uint64_t* set;

    uint64_t unused_stack;
    uint64_t used_stack;

    set_hash_function_t hash;
    set_cmp_function_t cmp;

    uint64_t size;
};

static inline uint64_t set_size_(set_t* set)
{
    return set->buf_allocated << 2;
}

set_t* set_create(set_hash_function_t hash, set_cmp_function_t cmp, uint64_t initial_size)
{
    set_t* set = malloc(sizeof(*set));
    if (set == NULL) {
        return NULL;
    }
    set->buf_allocated = initial_size;
    set->unused_stack = -1;
    set->used_stack = -1;
    set->size = 0;
    set->hash = hash;
    set->cmp = cmp;

    set->buf = malloc(initial_size * sizeof(*set->buf));
    set->set = malloc(set_size_(set) * sizeof(*set->set));
    memset(set->set, 0xFF, set_size_(set) * sizeof(*set->set));
    if (set->buf == NULL || set->set == NULL) {
        free(set->buf);
        free(set->set);
        free(set);
        return NULL;
    }
    stack_push_buf_(set, &set->unused_stack, 0, set->buf_allocated);
    return set;
}

void set_free(set_t* set)
{
    if (set == NULL) {
        return;
    }
    free(set->buf);
    free(set->set);
    free(set);
}

static inline void stack_push_(set_t* set, uint64_t* stack, uint64_t i)
{
    set->buf[i].prev = -1;
    if (*stack != -1) {
        set->buf[*stack].prev = i;
    }
    set->buf[i].next = *stack;
    *stack = i;
}

static inline void stack_push_buf_(set_t* set, uint64_t* stack, uint64_t iter, uint64_t size)
{
    for (; iter < size; iter++) {
        stack_push_(set, stack, iter);
    }
}

static inline uint64_t stack_pop_(set_t* set, uint64_t* stack)
{
    uint64_t r = *stack;
    *stack = set->buf[*stack].next;
    if (*stack != -1) {
        set->buf[*stack].prev = -1;
    }
    return r;
}

static inline uint64_t stack_pop_position_(set_t* set, uint64_t* stack, uint64_t pos)
{
    uint64_t prev = set->buf[pos].prev;
    uint64_t next = set->buf[pos].next;
    if (next != -1) {
        set->buf[next].prev = prev;
    }
    if (prev != -1) {
        set->buf[prev].next = next;
    }
    return pos;
}
static inline uint64_t set_position_(set_t* set, void* key, uint64_t* buf_position, uint64_t* chain)
{
    uint64_t hash = set->hash(key) % set_size_(set);
    *buf_position = set->set[hash];
    *chain = -1;
    while (*buf_position != -1 && !set->cmp(key, set->buf[*buf_position].key)) {
        *chain = *buf_position;
        *buf_position = set->buf[*buf_position].chain;
    }
    return hash;
}

static inline int check_realloc_(set_t* set)
{
    if (set->unused_stack != -1) {
        return 0;
    }

    uint64_t old_allocated = set->buf_allocated;
    set->buf_allocated <<= 1;
    entry_t* new_buf = realloc(set->buf, set->buf_allocated * sizeof(*new_buf));
    uint64_t* new_set = realloc(set->set, set_size_(set) * sizeof(*new_set));

    if (new_set == NULL || new_buf == NULL) {
        set->buf_allocated = old_allocated;
        free(new_buf);
        free(new_set);
        return -1;
    }

    set->buf = new_buf;
    set->set = new_set;
    memset(set->set, 0xFF, set_size_(set) * sizeof(*set->set));
    stack_push_buf_(set, &set->unused_stack, old_allocated, set->buf_allocated);

    uint64_t i;
    uint64_t chain;
    uint64_t buf_position;
    uint64_t hash;
    for (i = 0; i < old_allocated; i++) {
        hash = set_position_(set, set->buf[i].key, &buf_position, &chain);
        if (chain == -1) {
            set->set[hash] = i;
        } else {
            set->buf[chain].chain = i;
        }
        set->buf[i].chain = -1;
    }
    return 1;
}

int set_insert(set_t* set, void* key)
{
    {
        int err = check_realloc_(set);
        if (err < 0) {
            return err;
        }
    }
    uint64_t buf_position;
    uint64_t chain;
    uint64_t hash = set_position_(set, key, &buf_position, &chain);

    if (buf_position == -1) {
        buf_position = stack_pop_(set, &set->unused_stack);
        stack_push_(set, &set->used_stack, buf_position);
        set->buf[buf_position].key = key;
        set->size++;
        set->buf[buf_position].chain = -1;
        if (chain == -1) {
            set->set[hash] = buf_position;
        } else {
            set->buf[chain].chain = buf_position;
        }
    }
    return 0;
}

void set_delete(set_t* set, void* key)
{
    uint64_t chain;
    uint64_t buf_position;
    uint64_t hash = set_position_(set, key, &buf_position, &chain);
    if (buf_position != -1) {
        stack_pop_position_(set, &set->used_stack, buf_position);
        stack_push_(set, &set->unused_stack, buf_position);
        set->size--;
        if (chain == -1) {
            set->set[hash] = -1;
        } else {
            set->buf[chain].chain = -1;
        }
    }
}

void* set_iterator_key(set_t* set, set_iterator_t iter)
{
    return set->buf[iter].key;
}

set_iterator_t set_iterator_next(set_t* set, set_iterator_t iter)
{
    if (iter == -1) {
        return set->used_stack;
    }
    return set->buf[iter].next;
}

uint64_t set_size(set_t* set)
{
    return set->size;
}

//--------------------------------------------------------------------------------------------------------------------------------

uint64_t set_str_hash(void* key)
{
    char* s = key;
    uint64_t h = 86969;
    while (*s) {
        h = (h * 54059) ^ (s[0] * 76963);
        s++;
    }
    return h;
}

uint64_t set_str_jenkins(void* key)
{
    char* s = key;
    uint64_t hash = 0;
    char c;
    while ((c = *s++)) {
        hash += c;
        hash += hash << 10;
        hash ^= hash >> 6;
    }
    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;
    return hash;
}

bool set_str_cmp(void* a, void* b)
{
    return strcmp((char*)a, (char*)b) == 0;
}

bool set_contains(set_t* set, void* key)
{
    uint64_t buf_position;
    uint64_t chain;
    set_position_(set, key, &buf_position, &chain);
    return buf_position != -1;
}

uint64_t set_str_djb2(void* key)
{
    char* s = key;
    char c;
    uint64_t hash = 5381;
    while ((c = *s++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

bool set_i8_cmp(void* a, void* b)
{
    return *(int8_t*)a == *(int8_t*)b;
}

bool set_i16_cmp(void* a, void* b)
{
    return *(int16_t*)a == *(int16_t*)b;
}

bool set_i32_cmp(void* a, void* b)
{
    return *(int16_t*)a == *(int16_t*)b;
}

bool set_i64_cmp(void* a, void* b)
{
    return *(int64_t*)a == *(int64_t*)b;
}

bool set_u8_cmp(void* a, void* b)
{
    return *(uint8_t*)a == *(uint8_t*)b;
}

bool set_u16_cmp(void* a, void* b)
{
    return *(uint16_t*)a == *(uint16_t*)b;
}

bool set_u32_cmp(void* a, void* b)
{
    return *(uint16_t*)a == *(uint16_t*)b;
}

bool set_u64_cmp(void* a, void* b)
{
    return *(uint64_t*)a == *(uint64_t*)b;
}

uint64_t set_b64_self(void* key)
{
    return *(uint64_t*)key;
}

uint64_t set_b32_self(void* key)
{
    return *(uint32_t*)key;
}

uint64_t set_b16_self(void* key)
{
    return *(uint16_t*)key;
}
uint64_t set_b8_self(void* key)
{
    return *(uint8_t*)key;
}
