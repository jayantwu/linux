#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>


int main()
{
    pid_t pid;

    if ((pid = fork()) < 0) {
        printf("fork failed.\n");
    }
    else if (pid == 0) {
        // child
        if (execl("./basic", "basic", "abced", "12344", "dgdgd", NULL) == -1) {
            printf("execl failed.\n");
        }
    }
    for (int i = 0; i < 5; i++) {
        printf("parent...\n"); // if no "\n", it will display after child done.
        sleep(1);
    }
    if (waitpid(pid, NULL, 0) != pid)
        printf("waitpid error\n");
    exit(0);
}


/*
parent...
hello world
parent...
argv[0]: basic
parent...
argv[1]: abced
parent...
argv[2]: 12344
parent...
argv[3]: dgdgd

*/