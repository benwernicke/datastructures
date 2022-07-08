#include "map.h"

static inline int check_realloc_(map_t* map);
static inline uint64_t map_position_(map_t* map, void* key, uint64_t* buf_position, uint64_t* chain);

static inline uint64_t stack_pop_(map_t* map, uint64_t* stack);
static inline void stack_push_buf_(map_t* map, uint64_t* stack, uint64_t iter, uint64_t size);
static inline void stack_push_(map_t* map, uint64_t* stack, uint64_t i);
static inline uint64_t map_size_(map_t* map);

typedef struct entry_t entry_t;
struct entry_t {
    void* key;
    void* value;

    uint64_t next;
    uint64_t prev;
    uint64_t chain;
};

struct map_t {
    uint64_t buf_allocated;
    entry_t* buf;
    uint64_t* map;

    uint64_t unused_stack;
    uint64_t used_stack;

    map_hash_function_t hash;
    map_cmp_function_t cmp;

    uint64_t size;
};

static inline uint64_t map_size_(map_t* map)
{
    return map->buf_allocated << 2;
}

map_t* map_create(map_hash_function_t hash, map_cmp_function_t cmp, uint64_t initial_size)
{
    map_t* map = malloc(sizeof(*map));
    if (map == NULL) {
        return NULL;
    }
    map->buf_allocated = initial_size;
    map->unused_stack = -1;
    map->used_stack = -1;
    map->size = 0;
    map->hash = hash;
    map->cmp = cmp;

    map->buf = malloc(initial_size * sizeof(*map->buf));
    map->map = malloc(map_size_(map) * sizeof(*map->map));
    memset(map->map, 0xFF, map_size_(map) * sizeof(*map->map));
    if (map->buf == NULL || map->map == NULL) {
        free(map->buf);
        free(map->map);
        free(map);
        return NULL;
    }
    stack_push_buf_(map, &map->unused_stack, 0, map->buf_allocated);
    return map;
}

void map_free(map_t* map)
{
    if (map == NULL) {
        return;
    }
    free(map->buf);
    free(map->map);
    free(map);
}

static inline void stack_push_(map_t* map, uint64_t* stack, uint64_t i)
{
    map->buf[i].prev = -1;
    if (*stack != -1) {
        map->buf[*stack].prev = i;
    }
    map->buf[i].next = *stack;
    *stack = i;
}

static inline void stack_push_buf_(map_t* map, uint64_t* stack, uint64_t iter, uint64_t size)
{
    for (; iter < size; iter++) {
        stack_push_(map, stack, iter);
    }
}

static inline uint64_t stack_pop_(map_t* map, uint64_t* stack)
{
    uint64_t r = *stack;
    *stack = map->buf[*stack].next;
    if (*stack != -1) {
        map->buf[*stack].prev = -1;
    }
    return r;
}

static inline uint64_t stack_pop_position_(map_t* map, uint64_t* stack, uint64_t pos)
{
    uint64_t prev = map->buf[pos].prev;
    uint64_t next = map->buf[pos].next;
    if (next != -1) {
        map->buf[next].prev = prev;
    }
    if (prev != -1) {
        map->buf[prev].next = next;
    }
    return pos;
}
static inline uint64_t map_position_(map_t* map, void* key, uint64_t* buf_position, uint64_t* chain)
{
    uint64_t hash = map->hash(key) % map_size_(map);
    *buf_position = map->map[hash];
    *chain = -1;
    while (*buf_position != -1 && !map->cmp(key, map->buf[*buf_position].key)) {
        *chain = *buf_position;
        *buf_position = map->buf[*buf_position].chain;
    }
    return hash;
}

static inline int check_realloc_(map_t* map)
{
    if (map->unused_stack != -1) {
        return 0;
    }

    uint64_t old_allocated = map->buf_allocated;
    map->buf_allocated <<= 1;
    entry_t* new_buf = realloc(map->buf, map->buf_allocated * sizeof(*new_buf));
    uint64_t* new_map = realloc(map->map, map_size_(map) * sizeof(*new_map));

    if (new_map == NULL || new_buf == NULL) {
        map->buf_allocated = old_allocated;
        free(new_buf);
        free(new_map);
        return -1;
    }

    map->buf = new_buf;
    map->map = new_map;
    memset(map->map, 0xFF, map_size_(map) * sizeof(*map->map));
    stack_push_buf_(map, &map->unused_stack, old_allocated, map->buf_allocated);

    uint64_t i;
    uint64_t chain;
    uint64_t buf_position;
    uint64_t hash;
    for (i = 0; i < old_allocated; i++) {
        hash = map_position_(map, map->buf[i].key, &buf_position, &chain);
        if (chain == -1) {
            map->map[hash] = i;
        } else {
            map->buf[chain].chain = i;
        }
        map->buf[i].chain = -1;
    }
    return 1;
}

int map_insert(map_t* map, void* key, void* value)
{
    {
        int err = check_realloc_(map);
        if (err < 0) {
            return err;
        }
    }
    uint64_t buf_position;
    uint64_t chain;
    uint64_t hash = map_position_(map, key, &buf_position, &chain);

    if (buf_position == -1) {
        buf_position = stack_pop_(map, &map->unused_stack);
        stack_push_(map, &map->used_stack, buf_position);
        map->buf[buf_position].key = key;
        map->size++;
        map->buf[buf_position].chain = -1;
        if (chain == -1) {
            map->map[hash] = buf_position;
        } else {
            map->buf[chain].chain = buf_position;
        }
    }
    map->buf[buf_position].value = value;
    return 0;
}

void* map_get(map_t* map, void* key)
{
    uint64_t buf_position;
    uint64_t chain;
    map_position_(map, key, &buf_position, &chain);
    if (buf_position == -1) {
        return NULL;
    }
    return map->buf[buf_position].value;
}

void map_delete(map_t* map, void* key)
{
    uint64_t chain;
    uint64_t buf_position;
    uint64_t hash = map_position_(map, key, &buf_position, &chain);
    if (buf_position != -1) {
        stack_pop_position_(map, &map->used_stack, buf_position);
        stack_push_(map, &map->unused_stack, buf_position);
        map->size--;
        if (chain == -1) {
            map->map[hash] = -1;
        } else {
            map->buf[chain].chain = -1;
        }
    }
}

void* map_iterator_key(map_t* map, map_iterator_t iter)
{
    return map->buf[iter].key;
}

void* map_iterator_value(map_t* map, map_iterator_t iter)
{
    return map->buf[iter].value;
}

map_iterator_t map_iterator_next(map_t* map, map_iterator_t iter)
{
    if (iter == -1) {
        return map->used_stack;
    }
    return map->buf[iter].next;
}

uint64_t map_size(map_t* map)
{
    return map->size;
}

//--------------------------------------------------------------------------------------------------------------------------------

uint64_t map_str_hash(void* key)
{
    char* s = key;
    uint64_t h = 86969;
    while (*s) {
        h = (h * 54059) ^ (s[0] * 76963);
        s++;
    }
    return h;
}

uint64_t map_str_jenkins(void* key)
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

bool map_str_cmp(void* a, void* b)
{
    return strcmp((char*)a, (char*)b) == 0;
}

bool map_contains(map_t* map, void* key)
{
    uint64_t buf_position;
    uint64_t chain;
    map_position_(map, key, &buf_position, &chain);
    return buf_position != -1;
}

/*bool map_contains(map_t* map, void* key)*/
/*{*/
/*uint64_t i = map_position_(map, key);*/
/*return map->map[i] != -1;*/
/*}*/

uint64_t map_str_djb2(void* key)
{
    char* s = key;
    char c;
    uint64_t hash = 5381;
    while ((c = *s++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash;
}

bool map_i8_cmp(void* a, void* b)
{
    return *(int8_t*)a == *(int8_t*)b;
}

bool map_i16_cmp(void* a, void* b)
{
    return *(int16_t*)a == *(int16_t*)b;
}

bool map_i32_cmp(void* a, void* b)
{
    return *(int16_t*)a == *(int16_t*)b;
}

bool map_i64_cmp(void* a, void* b)
{
    return *(int64_t*)a == *(int64_t*)b;
}

bool map_u8_cmp(void* a, void* b)
{
    return *(uint8_t*)a == *(uint8_t*)b;
}

bool map_u16_cmp(void* a, void* b)
{
    return *(uint16_t*)a == *(uint16_t*)b;
}

bool map_u32_cmp(void* a, void* b)
{
    return *(uint16_t*)a == *(uint16_t*)b;
}

bool map_u64_cmp(void* a, void* b)
{
    return *(uint64_t*)a == *(uint64_t*)b;
}

uint64_t map_b64_self(void* key)
{
    return *(uint64_t*)key;
}

uint64_t map_b32_self(void* key)
{
    return *(uint32_t*)key;
}

uint64_t map_b16_self(void* key)
{
    return *(uint16_t*)key;
}
uint64_t map_b8_self(void* key)
{
    return *(uint8_t*)key;
}
