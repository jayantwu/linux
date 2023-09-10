#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>

#include <pthread.h>

void cleanup(void *arg) {
    printf("clean up\n");
}

void* t1_fun(void *arg) {
    printf("thread 1 exiting\n");
    pthread_exit((void*)1);
}

void* t2_fun(void *arg) {
    printf("thread 2 exiting\n");
    pthread_exit((void*)2);  // will return to pthread_jpin
}

int main()
{

    int err;
    pthread_t t1, t2;
    void * tret;

    err = pthread_create(&t1, NULL, t1_fun, NULL);
    if (err != 0) {
        printf("err\n");
        exit(-1);
    }
    err = pthread_create(&t2, NULL, t2_fun, NULL);
    if (err != 0) {
        printf("err\n");
        exit(-1);
    }
    err = pthread_join(t1, &tret);
    if (err != 0) {
        printf("err\n");
        exit(-1);        
    }
    printf("thread1 exit code val %ld\n", (long)tret);
    err = pthread_join(t2, &tret);
    if (err != 0) {
        printf("err\n");
        exit(-1);        
    }
    printf("thread2 exit code val %ld\n", (long)tret);
    exit(0);
}



