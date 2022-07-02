#define CBUILD
#include "cbuild.h"

#define FLAGS "-g -Wall -pedantic"

void compile_vector()
{
    compile_object("vector/vector.c", FLAGS, "vector/build/vector.o");
    compile_object("test/test.c", FLAGS, "vector/build/test.o");
    compile_object("vector/test_vector.c", FLAGS, "vector/build/test_vector.o");
    compile_object_directory("vector/main", FLAGS, "vector/build/");
}

void clean()
{
    printf("rm ./vector/build/*\n");
    system("rm ./vector/build/*");
    printf("rm ./vector/main\n");
    system("rm ./vector/main");

    printf("rm ./hashset/build/*\n");
    system("rm ./hashset/build/*");
    printf("rm ./hashset/main\n");
    system("rm ./hashset/main");
}

void compile_hashset()
{
    compile_object("hashset/hashset.c", FLAGS, "hashset/build/hashset.o");
    compile_object("test/test.c", FLAGS, "hashset/build/test.o");
    compile_object("hashset/test_hashset.c", FLAGS, "hashset/build/test_hashset.o");
    compile_object_directory("hashset/main", FLAGS, "hashset/build/");
}

int main(int argc, char** argv)
{
    auto_update();
    if (argc == 1) {
        compile_hashset();
        compile_vector();
    } else {
        if (strcmp(argv[1], "vector") == 0) {
            compile_vector();
        } else if (strcmp(argv[1], "hashset") == 0) {
            compile_hashset();
        } else if (strcmp(argv[1], "clean") == 0) {
            clean();
        } else if (strcmp(argv[1], "test-hashset") == 0) {
            compile_hashset();
            system("hashset/main");
        } else if (strcmp(argv[1], "test-vector") == 0) {
            compile_vector();
            system("vector/main");
        }
    }
}
