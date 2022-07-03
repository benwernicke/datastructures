#include "../test/test.h"
#include "path.h"

void test_cat()
{
    char t1[] = "lib/vector/vector.c";
    char* path_buf = NULL;
    uint64_t path_buf_len = 0;
    path_cat_or_panic(&path_buf, &path_buf_len, "lib", "vector", "vector.c");
    test_bool((char*)__func__, strcmp(t1, path_buf) == 0);

    free(path_buf);
}

int main(void)
{
    printf("PATH TEST\n");
    test_cat();
    test_total();
    return 0;
}
