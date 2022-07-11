#include "map.h"

static inline int check_realloc_(map_t* map);
static inline MAP_INT map_position_(map_t* map, MAP_INT hash, void* key, MAP_INT* buf_position, MAP_INT* chain);

static inline MAP_INT stack_pop_(map_t* map, MAP_INT* stack);
static inline void stack_push_buf_(map_t* map, MAP_INT* stack, MAP_INT iter, MAP_INT size);
static inline void stack_push_(map_t* map, MAP_INT* stack, MAP_INT i);
static inline MAP_INT map_size_(map_t* map);

typedef struct entry_t entry_t;
struct entry_t {
    void* key;
    void* value;

    MAP_INT next;
    MAP_INT prev;
    MAP_INT chain;
    MAP_INT hash;
};

struct map_t {
    MAP_INT buf_allocated;
    entry_t* buf;
    MAP_INT* map;

    MAP_INT unused_stack;
    MAP_INT used_stack;

    map_cmp_function_t cmp;

    MAP_INT size;
};

static inline MAP_INT map_size_(map_t* map)
{
    return map->buf_allocated << 2;
}

map_t* map_create(map_cmp_function_t cmp, MAP_INT initial_size)
{
    map_t* map = malloc(sizeof(*map));
    if (map == NULL) {
        return NULL;
    }
    map->buf_allocated = initial_size;
    map->unused_stack = -1;
    map->used_stack = -1;
    map->size = 0;
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

static inline void stack_push_(map_t* map, MAP_INT* stack, MAP_INT i)
{
    map->buf[i].prev = -1;
    if (*stack != -1) {
        map->buf[*stack].prev = i;
    }
    map->buf[i].next = *stack;
    *stack = i;
}

static inline void stack_push_buf_(map_t* map, MAP_INT* stack, MAP_INT iter, MAP_INT size)
{
    for (; iter < size; iter++) {
        stack_push_(map, stack, iter);
    }
}

static inline MAP_INT stack_pop_(map_t* map, MAP_INT* stack)
{
    MAP_INT r = *stack;
    *stack = map->buf[*stack].next;
    if (*stack != -1) {
        map->buf[*stack].prev = -1;
    }
    return r;
}

static inline MAP_INT stack_pop_position_(map_t* map, MAP_INT* stack, MAP_INT pos)
{
    MAP_INT prev = map->buf[pos].prev;
    MAP_INT next = map->buf[pos].next;
    if (next != -1) {
        map->buf[next].prev = prev;
    }
    if (prev != -1) {
        map->buf[prev].next = next;
    }
    return pos;
}
static inline MAP_INT map_position_(map_t* map, MAP_INT hash, void* key, MAP_INT* buf_position, MAP_INT* chain)
{
    hash %= map_size_(map);
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

    MAP_INT old_allocated = map->buf_allocated;
    map->buf_allocated <<= 1;
    entry_t* new_buf = realloc(map->buf, map->buf_allocated * sizeof(*new_buf));
    MAP_INT* new_map = realloc(map->map, map_size_(map) * sizeof(*new_map));

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

    MAP_INT i;
    MAP_INT chain;
    MAP_INT buf_position;
    MAP_INT map_pos;
    for (i = 0; i < old_allocated; i++) {
        map_pos = map_position_(map, map->buf[i].hash, map->buf[i].key, &buf_position, &chain);
        if (chain == -1) {
            map->map[map_pos] = i;
        } else {
            map->buf[chain].chain = i;
        }
        map->buf[i].chain = -1;
    }
    return 1;
}

void* map_get_or_insert(map_t* map, MAP_INT hash, void* key, void* value)
{
    MAP_INT buf_pos;
    MAP_INT chain;
    MAP_INT map_pos = map_position_(map, hash, key, &buf_pos, &chain);
    if (buf_pos != -1) {
        return map->buf[buf_pos].value;
    }
    {
        int err = check_realloc_(map);
        if (err < 0) {
            return NULL;
        }
        if (err > 0) {
            map_pos = map_position_(map, hash, key, &buf_pos, &chain);
        }
    }

    buf_pos = stack_pop_(map, &map->unused_stack);
    stack_push_(map, &map->used_stack, buf_pos);
    map->buf[buf_pos].key = key;
    map->size++;
    map->buf[buf_pos].chain = -1;
    map->buf[buf_pos].hash = hash;
    if (chain == -1) {
        map->map[map_pos] = buf_pos;
    } else {
        map->buf[chain].chain = buf_pos;
    }
    map->buf[buf_pos].value = value;
    return map->buf[buf_pos].value;
}

int map_insert(map_t* map, MAP_INT hash, void* key, void* value)
{
    {
        int err = check_realloc_(map);
        if (err < 0) {
            return err;
        }
    }
    MAP_INT buf_pos;
    MAP_INT chain;
    MAP_INT map_pos = map_position_(map, hash, key, &buf_pos, &chain);

    if (buf_pos == -1) {
        buf_pos = stack_pop_(map, &map->unused_stack);
        stack_push_(map, &map->used_stack, buf_pos);
        map->buf[buf_pos].key = key;
        map->size++;
        map->buf[buf_pos].chain = -1;
        map->buf[buf_pos].hash = hash;
        if (chain == -1) {
            map->map[map_pos] = buf_pos;
        } else {
            map->buf[chain].chain = buf_pos;
        }
    }
    map->buf[buf_pos].value = value;
    return 0;
}

void* map_get(map_t* map, MAP_INT hash, void* key)
{
    MAP_INT buf_position;
    MAP_INT chain;
    map_position_(map, hash, key, &buf_position, &chain);
    if (buf_position == -1) {
        return NULL;
    }
    return map->buf[buf_position].value;
}

void map_delete(map_t* map, MAP_INT hash, void* key)
{
    MAP_INT chain;
    MAP_INT buf_pos;
    MAP_INT map_pos = map_position_(map, hash, key, &buf_pos, &chain);
    if (buf_pos != -1) {
        stack_pop_position_(map, &map->used_stack, buf_pos);
        stack_push_(map, &map->unused_stack, buf_pos);
        map->size--;
        if (chain == -1) {
            map->map[map_pos] = -1;
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

MAP_INT map_size(map_t* map)
{
    return map->size;
}

bool map_contains(map_t* map, MAP_INT hash, void* key)
{
    MAP_INT buf_position;
    MAP_INT chain;
    map_position_(map, hash, key, &buf_position, &chain);
    return buf_position != -1;
}

MAP_INT map_str_djb2(void* key)
{
    char* s = key;
    char c;
    MAP_INT hash = 5381;
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
    return *(MAP_INT*)a == *(MAP_INT*)b;
}

MAP_INT map_b64_self(void* key)
{
    return *(MAP_INT*)key;
}

MAP_INT map_b32_self(void* key)
{
    return *(uint32_t*)key;
}

MAP_INT map_b16_self(void* key)
{
    return *(uint16_t*)key;
}
MAP_INT map_b8_self(void* key)
{
    return *(uint8_t*)key;
}

MAP_INT map_ptr_self(void* key)
{
    return (MAP_INT)(uintptr_t)key;
}

bool map_ptr_cmp(void* a, void* b)
{
    return a == b;
}
