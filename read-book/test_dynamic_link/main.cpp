#include "hello.h"
#include <unistd.h>

int main() {
    say();
    sleep(-1);
    return 0;
}


/*
[root@9d0875627904 build]# ./bin/run_main 
hello world
[root@9d0875627904 build]# ls lib/ -l
total 72
-rwxr-xr-x 1 root root 71056 Nov 21 12:06 libhellolib.so
[root@9d0875627904 build]# 
[root@9d0875627904 build]# cmake ..
-- Configuring done
-- Generating done
-- Build files have been written to: /usr/local/wujiayang/linux/read-book/test_dynamic_link/build
[root@9d0875627904 build]# make
Consolidate compiler generated dependencies of target hellolib
[ 50%] Building CXX object CMakeFiles/hellolib.dir/hello.cpp.o
[100%] Linking CXX shared library lib/libhellolib.so
[100%] Built target hellolib
[root@9d0875627904 build]# ls -l lib/
total 72
-rwxr-xr-x 1 root root 71056 Nov 21 12:08 libhellolib.so
[root@9d0875627904 build]# ./bin/run_main 
hello world, new
[root@9d0875627904 build]# ls -l bin/
total 80
-rwxr-xr-x 1 root root 79488 Nov 21 12:06 run_main

*/