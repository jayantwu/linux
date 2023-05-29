#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>

int main()
{
    #define BUFFSIZE 4096
    int n;
    char buf[BUFFSIZE];

    while ((n=read(STDIN_FILENO, buf, BUFFSIZE)) > 0) {
        //printf("n: %d, content: %s", n, buf);

        if (write(STDOUT_FILENO, buf, n) != n) {
            printf("write err.\n");
        }
    }
    if (n < 0) {
        printf("read error.\n");
    }
    return 0;
}