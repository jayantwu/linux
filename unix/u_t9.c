#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>

static void sig_usr(int);
int main()
{

    if (signal(SIGUSR1, sig_usr) == SIG_ERR) {
        printf("cant catch SIGUSR1\n");
    }
    if (signal(SIGUSR2, sig_usr) == SIG_ERR) {
        printf("cant catch SIGUSR2\n");
    }
    for (;;)
        pause();
    exit(0);
}

static void sig_usr(int signo) {
    if (signo == SIGUSR1) {
        printf("received SIGUSR1\n");
    }
    else if (signo == SIGUSR2) {
        printf("received SIGUSR2\n");
    }
    else {
        printf("received signal %d\n", signo);
    }
}


