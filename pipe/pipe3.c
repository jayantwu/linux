#include<stdio.h>
#include<unistd.h> 
#include<stdlib.h>
#include<string.h>
int main()
{
	int fd[2];
	int ret;
	int pid; 
	char writebuf[]="hello linux";
	char readbuf[128]={
		0
	};
	ret = pipe(fd);
	if (ret < 0)
	{
		printf("creat pipe failure\n");
		return -1;
	}
	pid = fork();
	if (pid == 0)  //child
	{
		//sleep(3);
		read(fd[0], readbuf, 128);
		printf("child read %s\n", readbuf);
	}
	//printf("creat pipe sucess fd[0]=%d, fd[1]=%d\n", fd[0], fd[1]);
	if (pid > 0)
	{
		write(fd[1], writebuf, sizeof(writebuf));
		printf("parent write\n");
		sleep(3);
	}
	
	close(fd[0]);
	close(fd[1]);
	
	return 0;
	
	
}
