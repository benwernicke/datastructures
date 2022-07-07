#include "sv.h"

void sv_init(sv_t* sv, char* s, uint64_t len)
{
    *sv = (sv_t) {
        .s = s,
        .len = len,
    };
}

void sv_init_from_cstr(sv_t* sv, char* s)
{
    sv_init(sv, s, strlen(s));
}

sv_t* sv_create(char* s, uint64_t len)
{
    sv_t* sv = malloc(sizeof(*sv));
    if (sv == NULL) {
        return NULL;
    }
    sv_init(sv, s, len);
    return sv;
}

sv_t* sv_create_from_cstr(char* s)
{
    return sv_create(s, strlen(s));
}

void sv_chop_left(sv_t* sv)
{
    sv->len--;
    sv->s++;
}

void sv_chop_right(sv_t* sv)
{
    sv->len--;
}

void sv_nchop_left(sv_t* sv, uint64_t n)
{
    sv->len -= n;
    sv->s += n;
}

void sv_nchop_right(sv_t* sv, uint64_t n)
{
    sv->len -= n;
}

void sv_trim_left(sv_t* sv)
{
    while (sv->len > 0 && isspace(*sv->s)) {
        sv_chop_left(sv);
    }
}

void sv_trim_right(sv_t* sv)
{
    while (sv->len > 0 && isspace(*(sv->s + sv->len - 1))) {
        sv_chop_right(sv);
    }
}

void sv_trim(sv_t* sv)
{
    sv_trim_left(sv);
    sv_trim_right(sv);
}

void sv_trim_left_if(sv_t* sv, sv_predicate_function_t p)
{
    while (sv->len > 0 && p(*sv->s)) {
        sv_chop_left(sv);
    }
}

void sv_trim_right_if(sv_t* sv, sv_predicate_function_t p)
{
    while (sv->len > 0 && p(*sv->s + sv->len - 1)) {
        sv_chop_left(sv);
    }
}

void sv_trim_if(sv_t* sv, sv_predicate_function_t p)
{
    sv_trim_left_if(sv, p);
    sv_trim_right_if(sv, p);
}

void sv_split_by_char(sv_t* dest, sv_t* src, char c)
{
    sv_init(dest, src->s, 0);
    while (src->len > 0 && *src->s != c) {
        dest->len++;
        sv_chop_left(src);
    }
    if (src->len > 0) {
        sv_chop_left(src);
    }
}

int sv_bigcmp(sv_t* a, sv_t* b)
{
    if (a->len > b->len) {
        return 1;
    }
    if (b->len < a->len) {
        return -1;
    }
    return memcmp(a->s, b->s, a->len);
}

bool sv_cmp(sv_t* a, sv_t* b)
{
    if (a->len != b->len) {
        return 0;
    }
    return memcmp(a->s, b->s, a->len) == 0;
}

bool sv_ncmp(sv_t* a, sv_t* b, uint64_t n)
{
    return memcmp(a->s, b->s, n) == 0;
}

int sv_bigncmp(sv_t* a, sv_t* b, uint64_t n)
{
    return memcmp(a->s, b->s, n);
}

void sv_split_by_sv(sv_t* dest, sv_t* src, sv_t* sv)
{
    sv_init(dest, src->s, 0);

    while (src->len >= sv->len && !sv_ncmp(src, sv, sv->len)) {
        dest->len++;
        sv_chop_left(src);
    }
    if (src->len < sv->len) {
        dest->len += src->len;
        sv_nchop_left(src, src->len);
    } else {
        sv_nchop_left(src, sv->len);
    }
}
