#!/bin/zsh
rm -f ./test/test0 ./test/test1 ./test/test2 ./test/test3 ./test/test4 ./test/test5 ./test/test_mutex
gcc -shared -o libft_malloc.so ./libft/libft.a -I ./libft/include -I ./include src/realloc.c src/malloc.c src/init_memory.c src/allocate_memory.c src/show_alloc_mem.c src/misc.c src/free.c -Wall -fPIC
gcc test/test0.c -I ./include -I ./libft/include -L ./ -lft_malloc -o test/test0 -Wall
gcc test/test1.c -I ./include -I ./libft/include -L ./ -lft_malloc -o test/test1 -Wall
gcc test/test2.c -I ./include -I ./libft/include -L ./ -lft_malloc -o test/test2 -Wall
gcc test/test3.c -I ./include -I ./libft/include -L ./ -lft_malloc -o test/test3 -Wall
gcc test/test4.c -I ./include -I ./libft/include -L ./ -lft_malloc -o test/test4 -Wall
gcc test/test5.c -I ./include -I ./libft/include -L ./ -lft_malloc -o test/test5 -Wall
gcc -pthread test/test_mutex.c -I ./include -I ./libft/include -L ./ -lft_malloc -o test/test_mutex -Wall
