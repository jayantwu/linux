#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>
#include<sys/msg.h>
#include<string.h>
struct msgbuf
{
  long type;
  char voltage[124];
  char ID[4];
};
int main(int argc, char** argv)
{
  int msgid;
  int readret;
  int key;
  int pid;
  struct msgbuf sendbuf,rcvbuf;
  key = ftok("./b.c", 'a');
  if (key < 0)
  {
	printf("creat key fail\n");
	return -1;
  }
  msgid = msgget(key, IPC_CREAT|0777);
  if (msgid < 0)
  {
    printf("create msgq failure\n");
    return -1;
  }
  printf("create msgq sucess msgid=%d\n", msgid);
  system("ipcs -q");
  pid = fork();
  if (pid < 0)
  {
	printf("creat multi process fail\n");
	return -1;
  }
  
  if(pid == 0)  //child process write
  {
  //start write
    sendbuf.type = 200;
    while (1)
    {
      memset(sendbuf.voltage,0,124);//clear buffer
      printf("please input message:\n");
      fgets(sendbuf.voltage,124,stdin);
      msgsnd(msgid,(void *)&sendbuf, strlen(sendbuf.voltage), 0);
    }
  }
  if (pid > 0) //parent process read
  {
    while(1)
	{
	  memset(rcvbuf.voltage, 0, 124);
	  msgrcv(msgid, (void*)&rcvbuf, 124, 100,0);
	  printf("receive message from msg queue:%s", rcvbuf.voltage);
    }
  }
  msgctl(msgid, IPC_RMID,NULL);
  system("ipcs -q");
  return 0;
}