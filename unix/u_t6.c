#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

int globvar = 6;
char buf[] = "a write to stdout\n";

int main()
{
    pid_t pid;

    if ((pid = fork()) < 0) {
        printf("fork failed.\n");
    }
    else if (pid == 0) {
        // child
        if ((pid = fork()) < 0) {
            printf("fork failed.\n");
        }
        else if (pid > 0) {
            exit(0);
        }
        sleep(10);
        printf("second child, parent pid = %ld\n", (long)getppid());
        exit(0);
    }

    if (waitpid(pid, NULL, 0) != pid)
        printf("waitpid error\n");
    exit(0);

    return 0;
}