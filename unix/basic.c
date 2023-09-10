#include<stdio.h>
#include <unistd.h>

int main(int argc, char** argv) {
    printf("hello world\n");
    for (int i = 0; i < argc; i++) {
        sleep(1);
        printf("argv[%d]: %s\n", i, argv[i]);
    }
    return 0;
}