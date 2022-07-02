#ifndef TEST_H
#define TEST_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>


void test_int_eq(char* desc, int64_t a, int64_t b);
void test_uint_eq(char* desc, uint64_t a, uint64_t b);
void test_total();
void test_bool(char* desc, bool b);

#endif
