#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>
#include<sys/msg.h>
int main(int argc, char** argv)
{
  int msgid;
  msgid = msgget(IPC_PRIVATE, 0777);
  if (msgid < 0)
  {
    printf("create msgq failure\n");
    return -1;
  }
  printf("create msgq sucess msgid=%d\n", msgid);
  system("ipcs -q");
  msgctl(msgid, IPC_RMID,NULL);
  system("ipcs -q");
  return 0;
}


