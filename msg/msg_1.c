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
  struct msgbuf sendbuf;
  struct msgbuf rcvbuf;
  msgid = msgget(IPC_PRIVATE, 0777);
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
  printf("please input message:\n");
  fgets(sendbuf.voltage,124,stdin);
  msgsnd(msgid,(void *)&sendbuf, strlen(sendbuf.voltage), 0);
  
  //start read from msg queue
  memset(rcvbuf.voltage, 0, 124);
  readret = msgrcv(msgid, (void*)&rcvbuf, 124, 100, 0);
  printf("recv:%s",rcvbuf.voltage);
  printf("readret=%d\n", readret);
  
  msgctl(msgid, IPC_RMID,NULL);
  system("ipcs -q");
  return 0;
}


