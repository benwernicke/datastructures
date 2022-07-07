#ifndef SV_H
#define SV_H

#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <ctype.h>
#include <stdbool.h>

typedef bool (*sv_predicate_function_t)(char c);

typedef struct sv_t sv_t;
struct sv_t{
    uint64_t len;
    char* s;
};

void sv_split_by_sv(sv_t* dest, sv_t* src, sv_t* sv);
int sv_bigncmp(sv_t* a, sv_t* b, uint64_t n);
bool sv_ncmp(sv_t* a, sv_t* b, uint64_t n);
bool sv_cmp(sv_t* a, sv_t* b);
int sv_bigcmp(sv_t* a, sv_t* b);
void sv_split_by_char(sv_t* dest, sv_t* src, char c);
void sv_trim_if(sv_t* sv, sv_predicate_function_t p);
void sv_trim_right_if(sv_t* sv, sv_predicate_function_t p);
void sv_trim_left_if(sv_t* sv, sv_predicate_function_t p);
void sv_trim(sv_t* sv);
void sv_trim_right(sv_t* sv);
void sv_trim_left(sv_t* sv);
void sv_nchop_right(sv_t* sv, uint64_t n);
void sv_nchop_left(sv_t* sv, uint64_t n);
void sv_chop_right(sv_t* sv);
void sv_chop_left(sv_t* sv);
sv_t* sv_create_from_cstr(char* s);
sv_t* sv_create(char* s, uint64_t len);
void sv_init_from_cstr(sv_t* sv, char* s);
void sv_init(sv_t* sv, char* s, uint64_t len);

#endif
