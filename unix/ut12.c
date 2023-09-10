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
    printf("clean up %s\n", (char*)arg);
}

void* t1_fun(void *arg) {
    printf("thread 1 start\n");
    pthread_cleanup_push(cleanup, "thread 1 first handler");
    pthread_cleanup_push(cleanup, "thread 1 second handler");
    printf("thread 1 push complete\n");
    if (arg)
        return ((void*)1);
    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);
    return (void*)1;
}

void* t2_fun(void *arg) {
    printf("thread 2 start\n");
    pthread_cleanup_push(cleanup, "thread 2 first handler");
    pthread_cleanup_push(cleanup, "thread 2 second handler");
    printf("thread 2 push complete\n");
    if (arg)
        pthread_exit((void*)2);
    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);
    pthread_exit((void*)2);
}

int main()
{

    int err;
    pthread_t t1, t2;
    void * tret;

    err = pthread_create(&t1, NULL, t1_fun, (void*)1);
    if (err != 0) {
        printf("err\n");
        exit(-1);
    }
    err = pthread_create(&t2, NULL, t2_fun, (void*)2);
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



