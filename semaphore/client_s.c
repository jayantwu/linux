#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/sem.h>
//#include<semaphore.h>

//sem_t sem;
union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO(Linux-specific) */
    };
/*
struct sembuf{	
    unsigned short sem_num;  // semaphore number 
    short          sem_op;   // semaphore operation   +1  v 操作 释放资源  -1 p操作占用资源
    short          sem_flg;  // operation flags 
} */

int semid;
union semun mysemun;
struct sembuf mysembuf;

int main()
{
	int i;
	char str[] = "hello linux\n";
    int key = ftok("./a.c", 'a');
    if (key < 0)
    {
	    printf("creat key fail\n");
	    return -1;
    }
	semid = semget(key, 3, IPC_CREAT |0777);
	if (semid < 0)
	{
		printf("creat semaphore failure\n");
		return -2;
	}
	printf("creat semaphore success\n");
	system("ipcs -s");
	//初始化信号量
	mysemun.val = 0;  //初始化为0
	//初始化  int semctl(int semid, int semnum, int cmd, ...);
	semctl(semid, 0, SETVAL, mysemun);
	//sem_init(&sem,0,0);
	mysembuf.sem_num = 0;
	mysembuf.sem_flg = 0;
	//second  p 操作
	mysembuf.sem_op = -1;
	semop(semid, &mysembuf, 1);
	
	for (i=0; i < 10; i++)
	{
		usleep(100);
		printf("client  fun print  i=%d\n", i);
	}
	//v操作
	//sem_post(&sem);
	mysembuf.sem_op = 1;
	semop(semid, &mysembuf, 1);
	while(1);
	return 0;
}
	
	
	
	
