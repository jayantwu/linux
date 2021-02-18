#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>

int main(int argc, char** argv)
{
  int pid;
  pid = fork();
  if (pid > 0)
  {
    sleep(8);
    if (waitpid(pid, NULL, WNOHANG)==0)
    {
      kill(pid, 9);
    }
    wait(NULL);
    while(1);
  }

  if (pid == 0)
  {
    printf("raise function before\n");
    raise(SIGTSTP);
    printf("raise fuunction after\n");
    exit(0);
  }
  return 0;
}


