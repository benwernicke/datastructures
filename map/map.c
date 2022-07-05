#include "map.h"

static ds_error_t check_realloc_(map_t* map);
static uint64_t map_position_(map_t* map, void* key);
static uint64_t stack_pop_(map_t* map, uint64_t* stack);
static void stack_push_buf_(map_t* map, uint64_t* stack, uint64_t iter, uint64_t size);
static void stack_push_(map_t* map, uint64_t* stack, uint64_t i);
static uint64_t map_size_(map_t* map);

typedef struct entry_t entry_t;
struct entry_t {
    void* key;
    void* value;

    uint64_t next;
};

struct map_t {
    uint64_t buf_allocated;
    entry_t* buf;
    uint64_t* map;

    uint64_t unused_stack;
    uint64_t used_stack;

    map_hash_function_t hash;
    map_cmp_function_t cmp;
};

static uint64_t map_size_(map_t* map)
{
    return map->buf_allocated << 2;
}

ds_error_t map_create(map_t** map, map_hash_function_t hash, map_cmp_function_t cmp, uint64_t initial_size)
{
    *map = malloc(sizeof(**map));
    if (*map == NULL) {
        return BAD_ALLOC;
    }
    (*map)->buf_allocated = initial_size;
    (*map)->unused_stack = -1;
    (*map)->used_stack = -1;
    (*map)->hash = hash;
    (*map)->cmp = cmp;

    (*map)->buf = malloc(initial_size * sizeof(*(*map)->buf));
    (*map)->map = malloc(map_size_(*map) * sizeof(*(*map)->map));
    memset((*map)->map, 0xFF, map_size_(*map) * sizeof(*(*map)->map));
    if ((*map)->buf == NULL || (*map)->map == NULL) {
        free((*map)->buf);
        free((*map)->map);
        free(*map);
        return BAD_ALLOC;
    }
    stack_push_buf_(*map, &(*map)->unused_stack, 0, (*map)->buf_allocated);
    return SUCCESS;
}

ds_error_t map_free(map_t* map)
{
    if (map == NULL) {
        return SUCCESS;
    }
    free(map->buf);
    free(map->map);
    free(map);
    return SUCCESS;
}

static void stack_push_(map_t* map, uint64_t* stack, uint64_t i)
{
    map->buf[i].next = *stack;
    /*map->stack = i;*/
    *stack = i;
}

static void stack_push_buf_(map_t* map, uint64_t* stack, uint64_t iter, uint64_t size)
{
    for (; iter < size; iter++) {
        stack_push_(map, stack, iter);
    }
}

static uint64_t stack_pop_(map_t* map, uint64_t* stack)
{
    uint64_t r = *stack;
    *stack = map->buf[*stack].next;
    return r;
}

static uint64_t map_position_(map_t* map, void* key)
{
    uint64_t i = map->hash(key) % map_size_(map);
    while (map->map[i] != -1 && !map->cmp(key, map->buf[map->map[i]].key)) {
        i = (i + 1) % map_size_(map);
    }
    return i;
}

static ds_error_t check_realloc_(map_t* map)
{
    if (map->unused_stack != -1) {
        return SUCCESS;
    }

    uint64_t old_allocated = map->buf_allocated;
    map->buf_allocated <<= 1;
    entry_t* new_buf = realloc(map->buf, map->buf_allocated * sizeof(*new_buf));
    uint64_t* new_map = realloc(map->map, map_size_(map) * sizeof(*new_map));

    if (new_map == NULL || new_buf == NULL) {
        map->buf_allocated = old_allocated;
        free(new_buf);
        free(new_map);
        return BAD_ALLOC;
    }

    map->buf = new_buf;
    map->map = new_map;
    memset(map->map, 0xFF, map_size_(map) * sizeof(*map->map));
    stack_push_buf_(map, &map->unused_stack, old_allocated, map->buf_allocated);

    uint64_t i;
    for (i = 0; i < old_allocated; i++) {
        map->map[map_position_(map, map->buf[i].key)] = i;
    }
    return SUCCESS;
}

ds_error_t map_insert(map_t* map, void* key, void* value)
{
    {
        ds_error_t err = check_realloc_(map);
        if (err != SUCCESS) {
            return err;
        }
    }
    uint64_t i = map_position_(map, key);
    if (map->map[i] == -1) {
        uint64_t n = stack_pop_(map, &map->unused_stack);
        stack_push_(map, &map->used_stack, n);
        if (n == -1) {
            return BAD_ALLOC;
        }
        map->map[i] = n;
    }
    map->buf[map->map[i]].key = key;
    map->buf[map->map[i]].value = value;
    return SUCCESS;
}

// is NULL a valid value
ds_error_t map_get(map_t* map, void* key, void** value)
{
    uint64_t i = map_position_(map, key);
    if (map->map[i] == -1) {
        *value = NULL;
    } else {
        *value = map->buf[map->map[i]].value;
    }
    return SUCCESS;
}

ds_error_t map_delete(map_t* map, void* key)
{
    uint64_t i = map_position_(map, key);
    uint64_t n = map->map[i];
    if (n != -1) {
        stack_push_(map, &map->unused_stack, n);
        map->map[i] = -1;
    }

    {
        map_iterator_t prev = -1;
        map_iterator_t iter;
        map_iterator_t end;
        map_iterator_begin(map, &iter);
        map_iterator_end(map, &end);
        for (; iter != end; map_iterator_next(map, &iter)) {
            if (iter == i) {
                if (prev == -1) {
                    stack_pop_(map, &map->used_stack);
                } else {
                    map->buf[prev].next = map->buf[iter].next;
                }
            }
            prev = iter;
        }
    }

    return SUCCESS;
}

void* map_iterator_key(map_t* map, map_iterator_t iter)
{
    return map->buf[iter].key;
}

void* map_iterator_value(map_t* map, map_iterator_t iter)
{
    return map->buf[iter].value;
}

ds_error_t map_iterator_begin(map_t* map, map_iterator_t* iter)
{
    *iter = map->used_stack;
    return SUCCESS;
}

ds_error_t map_iterator_next(map_t* map, map_iterator_t* iter)
{
    *iter = map->buf[*iter].next;
    return SUCCESS;
}

ds_error_t map_iterator_end(map_t* map, map_iterator_t* end)
{
    *end = -1;
    return SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------

uint64_t map_simple_str_hash(void* key)
{
    char* s = key;
    uint64_t h = 86969;
    while (*s) {
        h = (h * 54059) ^ (s[0] * 76963);
        s++;
    }
    return h;

}

bool map_strcmp(void* a, void* b)
{
    return strcmp((char*)a, (char*)b) == 0;
}
