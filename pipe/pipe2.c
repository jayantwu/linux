#include<stdio.h>
#include<unistd.h> 
#include<stdlib.h>
//验证写阻塞 
int main()
{
	int fd[2];
	int ret;
	int pid;
	
	char writebuf[]="hello linux";
	char readbuf[128];
	int i = 0;
	ret = pipe(fd);
	if (ret < 0)
	{
		printf("creat pipe failure\n");
		return -1;
	}
	printf("creat pipe sucess fd[0]=%d, fd[1]=%d\n", fd[0], fd[1]);
	while(i<5500)
	{
		write(fd[1], writebuf, sizeof(writebuf));
		i++;
	}
	printf("write end\n");
    
	close(fd[0]);
	close(fd[1]);
	
	return 0;
	
	
}