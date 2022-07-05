#include "set.h"

struct set_t {
    uint64_t allocated;
    uint64_t used;
    void** table;

    set_hash_function_t hash;
    set_cmp_function_t cmp;
};

ds_error_t set_create(set_t** hs, set_hash_function_t hash, set_cmp_function_t cmp, uint64_t initial_size)
{
    *hs = malloc(sizeof(**hs));
    if (*hs == NULL) {
        return BAD_ALLOC;
    }
    (*hs)->table = calloc(initial_size << 2, sizeof(*(*hs)->table));
    if ((*hs)->table == NULL) {
        free((*hs)->table);
        free(*hs);
        *hs = NULL;
        return BAD_ALLOC;
    }
    (*hs)->allocated = initial_size << 2;
    (*hs)->used = 0;
    (*hs)->cmp = cmp;
    (*hs)->hash = hash;
    return SUCCESS;
}

static uint64_t set_get_key_pos_(set_t* hs, void* key)
{
    uint64_t i = hs->hash(key) % hs->allocated;
    while (hs->table[i] != NULL && !hs->cmp(key, hs->table[i])) {
        i = (i + 1) % hs->allocated;
    }
    return i;
}

static ds_error_t set_realloc_(set_t* hs)
{
    set_t* new_hs;
    ds_error_t err = set_create(&new_hs, hs->hash, hs->cmp, hs->allocated << 2);
    if (err != SUCCESS) {
        return err;
    }
    uint64_t size;
    err = set_size(hs, &size);
    if (err != SUCCESS) {
        return err;
    }
    void* keys[size];
    err = set_get_keys(hs, keys, size);
    if (err != SUCCESS) {
        return err;
    }
    uint64_t i;
    for (i = 0; i < size; i++) {
        err = set_set(new_hs, keys[i]);
        if (err != SUCCESS) {
            set_free(new_hs);
            return err;
        }
    }
    free(hs->table);
    hs->table = new_hs->table;
    hs->allocated = new_hs->allocated;
    hs->used = new_hs->used;
    free(new_hs);
    return SUCCESS;
}

ds_error_t set_set(set_t* hs, void* key)
{
    if (hs->used << 2 >= hs->allocated) {
        ds_error_t err = set_realloc_(hs);
        if (err != SUCCESS) {
            return err;
        }
    }
    uint64_t i = set_get_key_pos_(hs, key);
    if (hs->table[i] == NULL) {
        hs->table[i] = key;
        hs->used++;
    }
    return SUCCESS;
}

ds_error_t set_has(set_t* hs, void* key, bool* has_key)
{
    uint64_t i = set_get_key_pos_(hs, key);
    *has_key = hs->table[i] != NULL;
    return SUCCESS;
}

ds_error_t set_get_keys(set_t* hs, void** buf, uint64_t buf_len)
{
    uint64_t i = 0;
    void** ptr;
    for (ptr = hs->table; i < buf_len && ptr != hs->table + hs->allocated; ptr++) {
        if (*ptr != NULL) {
            buf[i++] = *ptr;
        }
    }
    return SUCCESS;
}

ds_error_t set_size(set_t* hs, uint64_t* size)
{
    *size = hs->used;
    return SUCCESS;
}

ds_error_t set_delete(set_t* hs, void* key)
{
    uint64_t i = set_get_key_pos_(hs, key);
    if (hs->table[i] != NULL) {
        hs->used--;
        hs->table[i] = NULL;
    }
    return SUCCESS;
}

ds_error_t set_free(set_t* hs)
{
    if (hs == NULL) {
        return SUCCESS;
    }
    free(hs->table);
    free(hs);
    return SUCCESS;
}
