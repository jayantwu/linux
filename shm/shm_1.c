#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>
#include<sys/shm.h>
int main(int argc, char** argv)
{
  int shmid;
  int key = ftok("./a.c",'a');
  if (key < 0)
  {
    printf("creat key fail\n");
    return -2;
  }
  shmid = shmget(key, 128,IPC_CREAT |0777);
  if (shmid < 0)
  {
    printf("create share memory failure\n");
    return -1;
  }
  printf("create share memory sucess shmid=%d\n", shmid);
  system("ipcs -m");
  return 0;
}


