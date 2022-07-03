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
    printf("rm ./vector/log.log*\n");
    system("rm ./vector/log.log*");

    printf("rm ./hashset/build/*\n");
    system("rm ./hashset/build/*");
    printf("rm ./hashset/main\n");
    system("rm ./hashset/main");
    printf("rm ./hashset/log.log*\n");
    system("rm ./hashset/log.log*");

    printf("rm ./buffer/build/*\n");
    system("rm ./buffer/build/*");
    printf("rm ./buffer/main\n");
    system("rm ./buffer/main");
    printf("rm ./buffer/log.log*\n");
    system("rm ./buffer/log.log*");
}

void compile_buffer()
{
    compile_object("buffer/buffer.c", FLAGS, "buffer/build/buffer.o");
    compile_object("test/test.c", FLAGS, "buffer/build/test.o");
    compile_object("buffer/test_buffer.c", FLAGS, "buffer/build/test_buffer.o");
    compile_object_directory("buffer/main", FLAGS, "buffer/build/");
}

void compile_hashset()
{
    compile_object("hashset/hashset.c", FLAGS, "hashset/build/hashset.o");
    compile_object("test/test.c", FLAGS, "hashset/build/test.o");
    compile_object("hashset/test_hashset.c", FLAGS, "hashset/build/test_hashset.o");
    compile_object_directory("hashset/main", FLAGS, "hashset/build/");
}

void valgrind()
{
    system("valgrind --show-leak-kinds=all --log-file=buffer/log.log --leak-check=full buffer/main");
    system("valgrind --show-leak-kinds=all --log-file=vector/log.log --leak-check=full vector/main");
    system("valgrind --show-leak-kinds=all --log-file=hashset/log.log --leak-check=full hashset/main");
}

int main(int argc, char** argv)
{
    auto_update();
    if (argc == 1) {
        compile_hashset();
        compile_vector();
        compile_buffer();
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
        } else if (strcmp(argv[1], "buffer") == 0) {
            compile_buffer();
        } else if (strcmp(argv[1], "test-buffer") == 0) {
            compile_buffer();
            system("buffer/main");
        } else if (strcmp(argv[1], "valgrind") == 0) {
            valgrind();
        } else {
            fprintf(stderr, "unrecognised option: %s\n", argv[1]);
        }
    }
}
