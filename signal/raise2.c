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
    if (waitpid(pid, NULL, WNOHANG)==0)  //非阻塞方式     父进程返回的是子进程的pid
    {
      kill(pid, 9);   //向子进程发送kill信号， 子进程结束之后，父进程调用wait()函数回收资源，否则子进程成为僵尸进程
    }
    wait(NULL);
    while(1);
  }

  if (pid == 0)
  {
    printf("raise function before\n");
    raise(SIGTSTP);  //暂停
    printf("raise fuunction after\n");
    exit(0);
  }
  return 0;
}


