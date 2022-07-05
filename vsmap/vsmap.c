#include "vsmap.h"

struct vsmap_t {
    buffer_t* values;
    map_t* map;
};

ds_error_t vsmap_create(vsmap_t** map, map_hash_function_t hash, map_cmp_function_t cmp, uint64_t initial_size, uint64_t value_size)
{
    *map = malloc(sizeof(**map));
    if (*map == NULL) {
        return BAD_ALLOC;
    }
    ds_error_t err;
    err = map_create(&(*map)->map, hash, cmp, initial_size);
    if (err != SUCCESS) {
        free(*map);
        return err;
    }
    err = buffer_create(&(*map)->values, initial_size, value_size);
    if (err != SUCCESS) {
        map_free((*map)->map);
        free(*map);
        return err;
    }
    return SUCCESS;
}

ds_error_t vsmap_free(vsmap_t* map)
{
    if (map == NULL) {
        return SUCCESS;
    }
    buffer_free(map->values);
    map_free(map->map);
    free(map);
    return SUCCESS;
}

ds_error_t vsmap_get(vsmap_t* map, void* key, void** value)
{
    ds_error_t err;
    err = map_get(map->map, key, value);
    if (err != SUCCESS) {
        return err;
    }

    if (*value == NULL) {
        return SUCCESS;
    }

    err = buffer_get(map->values, (uintptr_t)value - 1, value);
    if (err != SUCCESS) {
        return err;
    }

    return SUCCESS;
}

ds_error_t vsmap_insert(vsmap_t* map, void* key, void** value)
{
    ds_error_t err;
    err = map_get(map->map, key, value);
    if (err != SUCCESS) {
        return err;
    }

    if (*value == NULL) {
        err = buffer_more(map->values, value);
        uintptr_t size = 0;
        buffer_size(map->values, &size);
        err = map_insert(map->map, key, (void*)(size + 1));
        if (err != SUCCESS) {
            return err;
        }

        return SUCCESS;
    }

    err = buffer_get(map->values, (uintptr_t)value - 1, value);
    if (err != SUCCESS) {
        return err;
    }

    return SUCCESS;
}

ds_error_t vsmap_delete(vsmap_t* map, void* key)
{
    //not implemented
    exit(1);
}
