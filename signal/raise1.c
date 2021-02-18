#include<stdio.h>
#include<signal.h>
#include<unistd.h>
#include<sys/types.h>
#include<stdlib.h>

int main(int argc, char** argv)
{
  printf("raise before\n");
  raise(9);
  printf("rease after\n");
  return 0;
}


