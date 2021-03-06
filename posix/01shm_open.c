#include<unistd.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<string.h>
#include<signal.h>
#include <sys/mman.h>


#include<stdlib.h>
#include<stdio.h>
#include<errno.h> 

#define ERR_EXIT(m)\
	do\
	{\
		perror(m);\
		exit(EXIT_FAILURE);\		
	}while(1)
	
typedef struct stu
{
	char name[32];
	int age;
}STU;
int main()
{
	int shmid;
	shmid = shm_open("/xyz", O_CREAT|O_RDWR, 0666);
	if(shmid == -1)
	{
		ERR_EXIT("shm_open");
	}
	printf("shm_open sucess\n");
	if(ftruncate(shmid, sizeof(STU)) == -1)   //修改共享内存的大小 
		ERR_EXIT("ftruncate"); 
	//int fstat(int fd, struct stat *buf);	
	struct stat buf;
	if(fstat(shmid, &buf))
	{
		ERR_EXIT("fstat");
	}
	printf("size=%ld mode=%o\n", buf.st_size, buf.st_mode & 07777);
	close(shmid);   //直接调用close()   没有像mq一样有shm_close 
	return 0;
} 