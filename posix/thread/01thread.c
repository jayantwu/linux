#include<unistd.h>
#include<sys/types.h>

#include<fcntl.h>
#include<sys/stat.h>
#include<string.h>
#include<errno.h>

#include<stdlib.h>
#include<stdio.h>
#include<pthread.h>


#define ERR_EXIT(m)\
	do\
	{\
		perror(m);\
		exit(EXIT_FAILURE);\		
	}while(1)

void* thread_routine(void*arg)
{
	int i;
	for(i=0; i < 20; i++)
	{
		printf("B");
		fflush(stdout);
		usleep(20);
		if (i == 3)
			pthread_exit("ABC");   //线程退出状态通过join返回到主线程中 
	}
	sleep(5);
	return 0;	   //每个线程有errno 
}
int main()
{
	pthread_t tid;
	int ret;
	int i;
	if(ret = pthread_create(&tid, NULL, thread_routine, NULL)!=0)
	{
		fprintf(stderr, "pthread_create:%s\n", strerror(ret));
		exit(EXIT_FAILURE);
	}
	
	for(i=0; i < 20; i++)
	{
		printf("A");
		fflush(stdout);
		usleep(20);
	}
	//sleep(1);
	void* value;
	if (ret = pthread_join(tid, &value)!=0)  //等待新创建的线程的结束,不关注返回value时直接填NULL 
	{
		fprintf(stderr, "pthread_join:%s\n", strerror(ret));
		exit(EXIT_FAILURE);
	}
	printf("\n");
	printf("return msg=%s\n", (char*)value);
	return 0;
}
