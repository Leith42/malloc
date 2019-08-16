cmake_minimum_required(VERSION 3.14)
project(malloc C)

set(CMAKE_C_STANDARD 99)

add_library(malloc SHARED src/malloc.c include/malloc.h)

execute_process (
        COMMAND bash -c "cc ./test/test.c -L./build -l./include -lmalloc -o ./test/test"
        OUTPUT_VARIABLE outVar
)
