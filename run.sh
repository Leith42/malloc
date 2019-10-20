#!/usr/bin/zsh
export LD_PRELOAD="/home/leith/CLionProjects/malloc/libmalloc.so"
./test/test
unset LD_PRELOAD
