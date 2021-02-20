#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>
#include<sys/shm.h>

struct mybuf
{
	int pid;
	char buf[124];
};

void myfun(int signum)
{
	return;
}
int main(int argc, char** argv)
{
  int shmid;
  int pid;
  struct mybuf *p;
  int key;
  key = ftok("./a.c", 'a');
  if (key < 0)
  {
	printf("creat key fail\n");
	return -1;
  }
  
  shmid = shmget(key, 128,IPC_CREAT |0777);
  if (shmid < 0)
  {
    printf("create share memory failure\n");
    return -1;
  }
  printf("creat shm success shmid=%d\n", shmid);
  signal(SIGUSR2, myfun);
  p=(struct mybuf*)shmat(shmid, NULL, 0);
  if (p ==NULL)
  {
    printf("shmat function fail\n");
    return -3;
  }
  
  //get client pid
  p->pid = getpid(); //write server pid to shm;
  pause();  //wait client read server pid;
  pid = p->pid;
  kill(pid, SIGUSR1);
  
  while(1)
  {
    printf("server process start write\n");
    fgets(p->buf, 128, stdin);
    kill(pid,SIGUSR1);  //send signal 1 to client process to read data
    pause();//wait client to read shm
  }

  shmdt(p);
  shmctl(shmid, IPC_RMID, NULL);
  system("ipcs -m");
  return 0;
}