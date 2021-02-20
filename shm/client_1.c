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
  signal(SIGUSR1, myfun);
  p=(struct mybuf*)shmat(shmid, NULL, 0);
  if (p ==NULL)
  {
    printf("shmat function fail\n");
    return -3;
  }
  
  //get server pid
  pid = p->pid;
  
  p->pid = getpid(); //write client pid to shm;
  kill(pid, SIGUSR2);  //发送信号给server，server可读取client pid
  //client start read
  while(1)
  {
    pause();//wait server write data
	printf("client process receive data:%s\n", p->buf);
    kill(pid,SIGUSR2);  //send signal to server process to write data
    
  }

  shmdt(p);
  shmctl(shmid, IPC_RMID, NULL);
  system("ipcs -m");
  return 0;
}