#include "../test/test.h"
#include "cmp.h"

void test_cmp_int(void)
{
    {
        uint64_t a = 69L;
        uint64_t b = 69L;
        uint64_t c = 420L;
        test(cmp_u64(&a, &b));
        test(!cmp_u8(&a, &c));
    }

    {
        uint32_t a = 69;
        uint32_t b = 69;
        uint32_t c = 420;
        test(cmp_u32(&a, &b));
        test(!cmp_u32(&a, &c));
    }

    {
        uint16_t a = 69;
        uint16_t b = 69;
        uint16_t c = 420;
        test(cmp_u16(&a, &b));
        test(!cmp_u16(&a, &c));
    }

    {
        uint8_t a = 69;
        uint8_t b = 69;
        uint8_t c = 1;
        test(cmp_u8(&a, &b));
        test(!cmp_u8(&a, &c));
    }

    {
        int64_t a = 69;
        int64_t b = 69;
        int64_t c = 420;
        test(cmp_i64(&a, &b));
        test(!cmp_i8(&a, &c));
    }

    {
        int32_t a = 69;
        int32_t b = 69;
        int32_t c = 420;
        test(cmp_i32(&a, &b));
        test(!cmp_i32(&a, &c));
    }

    {
        int16_t a = 69;
        int16_t b = 69;
        int16_t c = 420;
        test(cmp_i16(&a, &b));
        test(!cmp_i16(&a, &c));
    }

    {
        int8_t a = 69;
        int8_t b = 69;
        int8_t c = 1;
        test(cmp_i8(&a, &b));
        test(!cmp_i8(&a, &c));
    }
}

void test_cmp_str(void)
{
    {
        char s1[] = "Hello World!\n";
        char s2[] = "Hello World!\n";
        char s3[] = "Hello Hell!\n";
        test(cmp_str(s1, s2));
        test(!cmp_str(s1, s3));
    }
}

int main(void)
{
    test_cmp_int();
    test_cmp_str();
    return 0;
}
