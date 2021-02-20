#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>
#include<sys/shm.h>
void myfun(int signum)
{
	return;
}
int main(int argc, char** argv)
{
  int shmid;
  int pid;
  char*p;
  shmid = shmget(IPC_PRIVATE, 128,IPC_CREAT |0777);
  if (shmid < 0)
  {
    printf("create share memory failure\n");
    return -1;
  }
  pid = fork();
  if (pid > 0)  //parent process
  {
    signal(SIGUSR2, myfun);
	p=(char*)shmat(shmid, NULL, 0);
    if (p ==NULL)
    {
      printf("shmat function fail\n");
      return -3;
    }
    while(1)
    {
      printf("parent process start write\n");
      fgets(p, 128, stdin);
      kill(pid,SIGUSR1);  //send signal to child process to read data
      pause();//wait
    }
  }
  if (pid == 0)  //child process
  {
    while(1)
    {
      signal(SIGUSR1,myfun);
	  p=(char*)shmat(shmid, NULL, 0);
	  if (p ==NULL)
      {
      printf("child process shmat function fail\n");
      return -3;
      }
	  pause();  //wait parent process write and wait a signal
      //start read shm;
      printf("child process share memory data:%s",p);
      kill(getppid(),SIGUSR2);//send signal to parent process
    }
  }
  shmdt(p);
  shmctl(shmid, IPC_RMID, NULL);
  system("ipcs -m");
  return 0;
}
