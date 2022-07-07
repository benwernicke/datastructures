#include "../test/test.h"
#include "sv.h"

void test_trim(void)
{
    char s[] = "  \t\n  Hello World  \n\n  ";
    char t[] = "Hello World";
    sv_t src;
    sv_t test;
    sv_init(&test, t, strlen(t));
    sv_init(&src, s, strlen(s));
    sv_trim(&src);

    test(sv_cmp(&test, &src));
}

void test_split(void)
{
    char s[] = "Hello World I am Ben";
    sv_t svs[20] = { 0 };
    uint64_t size = 0;
    sv_t src;
    sv_init(&src, s, strlen(s));

    while (src.len > 0) {
        sv_split_by_char(&svs[size++], &src, ' ');
    }
    test(size == 5);

    sv_t* Hello = sv_create_from_cstr("Hello");
    sv_t* World = sv_create_from_cstr("World");
    sv_t* I = sv_create_from_cstr("I");
    sv_t* am = sv_create_from_cstr("am");
    sv_t* Ben = sv_create_from_cstr("Ben");

    test(sv_cmp(Hello, &svs[0]));
    test(sv_cmp(World, &svs[1]));
    test(sv_cmp(I, &svs[2]));
    test(sv_cmp(am, &svs[3]));
    test(sv_cmp(Ben, &svs[4]));

    sv_init_from_cstr(&src, s);

    size = 0;
    while (src.len > 0) {
        sv_split_by_sv(&svs[size++], &src, World);
    }

    test(size == 2);

    sv_t* Hello_ = sv_create_from_cstr("Hello ");
    test(sv_cmp(Hello_, &svs[0]));

    sv_t* I_am_Ben = sv_create_from_cstr(" I am Ben");
    test(sv_cmp(I_am_Ben, &svs[1]));

    free(Hello);
    free(I_am_Ben);
    free(Hello_);
    free(World);
    free(I);
    free(am);
    free(Ben);
}

void test_create_cmp(void)
{
    char s[] = "Hello World!";
    char n[] = "Hello Hell!";

    sv_t* sv0 = sv_create(s, strlen(s));
    sv_t* sv1 = sv_create_from_cstr(s);

    sv_t* nv0 = sv_create(n, strlen(n));
    sv_t* nv1 = sv_create_from_cstr(n);

    test(sv_cmp(sv0, sv1));
    test(sv_cmp(nv0, nv1));
    test(!sv_cmp(sv0, nv0));
    test(sv_ncmp(sv0, nv0, 5));

    test(sv_bigcmp(sv0, sv1) == 0);
    test(sv_bigcmp(nv0, nv1) == 0);
    test(sv_bigcmp(sv0, nv0) != 0);
    test(sv_bigncmp(sv0, nv0, 5) == 0);

    free(sv0);
    free(sv1);
    free(nv0);
    free(nv1);
}

int main(void)
{
    test_create_cmp();
    test_split();
    test_trim();
    return 0;
}
