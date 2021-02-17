#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>

int main()
{
	
	int result = -1;
	int fd[2], nbytes;
	pid_t pid;
	char string[] = "hello pipe!\n";
	char readbuffer[80];
	int *write_fd = &fd[1];
	int *read_fd = &fd[0];
	printf("%d %d\n", *write_fd, *read_fd);
	result = pipe(fd);
	if (-1 == result)
	{
		printf("pipe creat failed\n");
		return -1;
	}
	pid = fork();
	if (-1 == pid)
	{
		printf("fork failed!\n");
		return -1;
	}
	
	if (0 == pid)
	{
		close(*read_fd);
		result = write(*write_fd, string, strlen(string));
		return 0;
	}
	else
	{
		close(*write_fd);
		result = read(*read_fd, readbuffer, sizeof(readbuffer));
		printf("receive %d bytes :%s\n", result, readbuffer);
	}
	return 0;
	
}
