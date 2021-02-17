#include<stdio.h>
#include<sys/ipc.h>
#include<sys/types.h>
#include<sys/msg.h>
int main()
{
	printf("%d %d", IPC_CREAT, IPC_EXCL);
} 