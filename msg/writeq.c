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
  struct msgbuf sendbuf;
  struct msgbuf rcvbuf;
  key = ftok("./a.c", 'a');
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
  //start write
  //init sendbuf
  sendbuf.type = 100;
  while (1)
  {
    memset(sendbuf.voltage,0,124);//clear buffer
    printf("please input message:\n");
    fgets(sendbuf.voltage,124,stdin);
    msgsnd(msgid,(void *)&sendbuf, strlen(sendbuf.voltage), 0);
  }
  

  msgctl(msgid, IPC_RMID,NULL);
  system("ipcs -q");
  return 0;
}
