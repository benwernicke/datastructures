#define CBUILD
#include "cbuild.h"

#define FLAGS "-g -Wall -pedantic"

void splint(void)
{
    system("splint -nullstate set/set.c > set/log.log");
    system("splint -nullstate vector/vector.c > vector/log.log");
    system("splint -nullstate buffer/buffer.c > buffer/log.log");
    system("splint -nullstate path/path.c > path/log.log");
    system("splint -nullstate path/path.c > path/log.log");
    system("splint -nullstate map/map.c > map/log.log");
    system("splint -nullstate sv/sv.c > sv/log.log");
    system("splint -nullstate cmp/cmp.c > cmp/log.log");
}

void compile_sv()
{
    compile_object("sv/sv.c", FLAGS, "sv/build/sv.o");
    compile_object("sv/test_sv.c", FLAGS, "sv/build/test_sv.o");
    compile_object_directory("sv/main", FLAGS, "sv/build/");
}

void compile_cmp()
{
    compile_object("cmp/cmp.c", FLAGS, "cmp/build/cmp.o");
    compile_object("cmp/test_cmp.c", FLAGS, "cmp/build/test_cmp.o");
    compile_object_directory("cmp/main", FLAGS, "cmp/build/");
}

void compile_vector()
{
    compile_object("vector/vector.c", FLAGS, "vector/build/vector.o");
    compile_object("vector/test_vector.c", FLAGS, "vector/build/test_vector.o");
    compile_object_directory("vector/main", FLAGS, "vector/build/");
}

void compile_map()
{
    compile_object("cmp/cmp.c", FLAGS, "map/build/cmp.o");
    compile_object("map/map.c", FLAGS, "map/build/map.o");
    compile_object("map/test_map.c", FLAGS, "map/build/test_map.o");
    compile_object_directory("map/main", FLAGS, "map/build/");
}

void compile_path()
{
    compile_object("path/path.c", FLAGS, "path/build/path.o");
    compile_object("path/test_path.c", FLAGS, "path/build/test_path.o");
    compile_object_directory("path/main", FLAGS, "path/build/");
}

#define clean_one(dir)                 \
    printf("rm ./" dir "/build/*\n");  \
    system("rm ./" dir "/build/*");    \
    printf("rm ./" dir "/main\n");     \
    system("rm ./" dir "/main");       \
    printf("rm ./" dir "/log.log*\n"); \
    system("rm ./" dir "/log.log*");

void clean()
{
    clean_one("vector");
    clean_one("map");
    clean_one("set");
    clean_one("buffer");
    clean_one("path");
    clean_one("sv");
    clean_one("cmp");

    printf("rm log.log\n");
    system("rm log.log");
}

void compile_buffer()
{
    compile_object("buffer/buffer.c", FLAGS, "buffer/build/buffer.o");
    compile_object("buffer/test_buffer.c", FLAGS, "buffer/build/test_buffer.o");
    compile_object_directory("buffer/main", FLAGS, "buffer/build/");
}

void compile_set()
{
    compile_object("cmp/cmp.c", FLAGS, "set/build/cmp.o");
    compile_object("set/set.c", FLAGS, "set/build/set.o");
    compile_object("set/test_set.c", FLAGS, "set/build/test_set.o");
    compile_object_directory("set/main", FLAGS, "set/build/");
}

void valgrind()
{
    system("valgrind --show-leak-kinds=all --log-file=buffer/log.log --leak-check=full buffer/main");
    system("valgrind --show-leak-kinds=all --log-file=vector/log.log --leak-check=full vector/main");
    system("valgrind --show-leak-kinds=all --log-file=set/log.log --leak-check=full set/main");
    system("valgrind --show-leak-kinds=all --log-file=path/log.log --leak-check=full path/main");
    system("valgrind --show-leak-kinds=all --log-file=map/log.log --leak-check=full map/main");
    system("valgrind --show-leak-kinds=all --log-file=sv/log.log --leak-check=full sv/main");
    system("valgrind --show-leak-kinds=all --log-file=cmp/log.log --leak-check=full cmp/main");

    printf("catting logs\n");
    system("touch log.log");

    system("echo ---BUFFER--- >> log.log");
    system("cat buffer/log.log >> log.log");
    system("rm buffer/log.log*");

    system("echo ---VECTOR--- >> log.log");
    system("cat vector/log.log >> log.log");
    system("rm vector/log.log*");

    system("echo ---MAP--- >> log.log");
    system("cat map/log.log >> log.log");
    system("rm map/log.log*");

    system("echo ---SET--- >> log.log");
    system("cat set/log.log >> log.log");
    system("rm set/log.log*");

    system("echo ---PATH--- >> log.log");
    system("cat path/log.log >> log.log");
    system("rm path/log.log*");

    system("echo ---SV--- >> log.log");
    system("cat sv/log.log >> log.log");
    system("rm sv/log.log*");
}

void compile_all()
{
    compile_set();
    compile_vector();
    compile_buffer();
    compile_path();
    compile_map();
    compile_sv();
    compile_cmp();
}

int main(int argc, char** argv)
{
    auto_update();
    if (argc == 1) {
        compile_all();
    } else {
        if (strcmp(argv[1], "vector") == 0) {
            compile_vector();
            system("vector/main");
        } else if (strcmp(argv[1], "set") == 0) {
            compile_set();
            system("set/main");
        } else if (strcmp(argv[1], "clean") == 0) {
            clean();
        } else if (strcmp(argv[1], "buffer") == 0) {
            compile_buffer();
            system("buffer/main");
        } else if (strcmp(argv[1], "valgrind") == 0) {
            clean();
            compile_all();
            valgrind();
        } else if (strcmp(argv[1], "path") == 0) {
            compile_path();
            system("path/main");
        } else if (strcmp(argv[1], "map") == 0) {
            compile_map();
            system("map/main");
        } else if (strcmp(argv[1], "sv") == 0) {
            compile_sv();
            system("sv/main");
        } else if (strcmp(argv[1], "cmp") == 0) {
            compile_cmp();
            system("cmp/main");
        } else if (strcmp(argv[1], "splint") == 0) {
            splint();
        } else {
            fprintf(stderr, "unknown option: %s\n", argv[1]);
        }
    }
}
