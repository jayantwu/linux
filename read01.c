#include <stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
int main()
{
	int fd = -1, i;
	ssize_t size = -1;
	char buf[10];
	char filename[] = "test.txt";
	fd = open(filename, O_RDONLY);
	printf("fd:%d\n", fd);
	while (size)
	{
		size = read(fd, buf, 10);
		if (-1 == size)
		{
			close(fd);
			printf("read file error occurs\n");
			return -1;
		}
		else
		{
			if (size > 0)
			{
				printf("read %d bytes:", size);
				printf("\"");
				for (i = 0; i < size; i++);
					printf("%c", *(buf+i));
				printf("\"\n");
             }
			 else
			 	printf("reach the end of file\n");	
		}
	}
	return 0;
}
