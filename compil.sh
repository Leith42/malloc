#!/usr/bin/zsh
rm -f ./test/test
gcc -shared -o libmalloc.so -Wl,--whole-archive ./libft/libft.a -Wl,--no-whole-archive -I ./libft/include -I ./include src/malloc.c src/init_memory.c src/allocate_memory.c src/show_alloc_mem.c src/misc.c src/free.c -Wall -fPIC
gcc test/test.c -I ./include -I ./libft/include -L ./ -lmalloc -o test/test -Wall
