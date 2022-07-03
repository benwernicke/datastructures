#include "hashset.h"

struct hashset_t {
    uint64_t allocated;
    uint64_t used;
    void** table;

    hashset_hash_function_t hash;
    hashset_cmp_function_t cmp;
};

ds_error_t hashset_create(hashset_t** hs, hashset_hash_function_t hash, hashset_cmp_function_t cmp, uint64_t initial_size)
{
    *hs = malloc(sizeof(**hs));
    if (*hs == NULL) {
        return DS_BAD_ALLOC;
    }
    (*hs)->table = calloc(initial_size << 2, sizeof(*(*hs)->table));
    if ((*hs)->table == NULL) {
        free((*hs)->table);
        free(*hs);
        *hs = NULL;
        return DS_BAD_ALLOC;
    }
    (*hs)->allocated = initial_size << 2;
    (*hs)->used = 0;
    (*hs)->cmp = cmp;
    (*hs)->hash = hash;
    return DS_SUCCESS;
}

static uint64_t hashset_get_key_pos_(hashset_t* hs, void* key)
{
    uint64_t i = hs->hash(key) % hs->allocated;
    while (hs->table[i] != NULL && !hs->cmp(key, hs->table[i])) {
        i = (i + 1) % hs->allocated;
    }
    return i;
}

static ds_error_t hashset_realloc_(hashset_t* hs)
{
    hashset_t* new_hs;
    ds_error_t err = hashset_create(&new_hs, hs->hash, hs->cmp, hs->allocated << 2);
    if (err != DS_SUCCESS) {
        return err;
    }
    uint64_t size;
    err = hashset_size(hs, &size);
    if (err != DS_SUCCESS) {
        return err;
    }
    void* keys[size];
    err = hashset_get_keys(hs, keys, size);
    if (err != DS_SUCCESS) {
        return err;
    }
    uint64_t i;
    for (i = 0; i < size; i++) {
        err = hashset_set(new_hs, keys[i]);
        if (err != DS_SUCCESS) {
            hashset_free(new_hs);
            return err;
        }
    }
    free(hs->table);
    hs->table = new_hs->table;
    hs->allocated = new_hs->allocated;
    hs->used = new_hs->used;
    free(new_hs);
    return DS_SUCCESS;
}

ds_error_t hashset_set(hashset_t* hs, void* key)
{
    if (hs->used << 2 >= hs->allocated) {
        ds_error_t err = hashset_realloc_(hs);
        if (err != DS_SUCCESS) {
            return err;
        }
    }
    uint64_t i = hashset_get_key_pos_(hs, key);
    if (hs->table[i] == NULL) {
        hs->table[i] = key;
        hs->used++;
    }
    return DS_SUCCESS;
}

ds_error_t hashset_has(hashset_t* hs, void* key, bool* has_key)
{
    uint64_t i = hashset_get_key_pos_(hs, key);
    *has_key = hs->table[i] != NULL;
    return DS_SUCCESS;
}

ds_error_t hashset_get_keys(hashset_t* hs, void** buf, uint64_t buf_len)
{
    uint64_t i = 0;
    void** ptr;
    for (ptr = hs->table; i < buf_len && ptr != hs->table + hs->allocated; ptr++) {
        if (*ptr != NULL) {
            buf[i++] = *ptr;
        }
    }
    return DS_SUCCESS;
}

ds_error_t hashset_size(hashset_t* hs, uint64_t* size)
{
    *size = hs->used;
    return DS_SUCCESS;
}

ds_error_t hashset_delete(hashset_t* hs, void* key)
{
    uint64_t i = hashset_get_key_pos_(hs, key);
    if (hs->table[i] != NULL) {
        hs->used--;
        hs->table[i] = NULL;
    }
    return DS_SUCCESS;
}

ds_error_t hashset_free(hashset_t* hs)
{
    if (hs == NULL) {
        return DS_SUCCESS;
    }
    free(hs->table);
    free(hs);
    return DS_SUCCESS;
}
