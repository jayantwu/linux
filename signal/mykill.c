#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>

int main(int argc, char** argv)
{
  int sig;
  int pid;
  if(argc < 3)
  {
    printf("please input param\n");
    return -1;
  }
  sig = atoi(argv[1]);
  pid = atoi(argv[2]);
  kill(pid, sig);
  return 0;
}


