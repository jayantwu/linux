#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>
#include<sys/sem.h>
int main(int argc, char** argv)
{
  int semid;
  semid = semget(IPC_PRIVATE,3, 0777);
  if (semid < 0)
  {
    printf("create semaphore failure\n");
    return -1;
  }
  printf("create semaphore sucess semid=%d\n", semid);
  system("ipcs -s");
  semctl(semid,0, IPC_RMID,NULL);
  system("ipcs -s");
  return 0;
}


