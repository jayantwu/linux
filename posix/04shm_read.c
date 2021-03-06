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
	shmid = shm_open("/xyz", O_RDONLY, 0);
	if(shmid == -1)
	{
		ERR_EXIT("shm_open");
	}
	printf("shm_open sucess\n");
	//int fstat(int fd, struct stat *buf);	
	struct stat buf;
	if(fstat(shmid, &buf))   //获取共享内存对象信息 
	{
		ERR_EXIT("fstat");
	}
	printf("size=%ld mode=%o\n", buf.st_size, buf.st_mode & 07777);
	
	STU *p;
	p = mmap(NULL, buf.st_size, PROT_READ, MAP_SHARED, shmid, 0);//保护模式，第一个参数NULL，由内核分配 
	if (p == MAP_FAILED)  //失败返回-1 
	{
		ERR_EXIT("mmap");
	}   
	//读共享内存数据 
	printf("name=%s age=%d\n", p->name, p->age);
	close(shmid);   //直接调用close()   没有像mq一样有shm_close 
	return 0;
} 