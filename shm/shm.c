#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>
#include<sys/shm.h>
int main(int argc, char** argv)
{
  int shmid;
  shmid = shmget(IPC_PRIVATE, 128, 0777);
  if (shmid < 0)
  {
    printf("create share memory failure\n");
    return -1;
  }
  printf("create share memory sucess shmid=%d\n", shmid);
  system("ipcs -m");
  return 0;
}


