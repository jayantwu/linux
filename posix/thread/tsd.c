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

typedef struct tsd
{
	pthread_t tid;
	char *str;
}tsd_t;

pthread_key_t key_tsd;
pthread_once_t once_control = PTHREAD_ONCE_INIT;
void destroy_routine(void*value)
{
	printf("destroy...\n");
	free(value);
}

void once_routine()
{
	pthread_key_create(&key_tsd, destroy_routine);
	printf("key init...\n");
}


void *thread_routine(void *arg)
{
	pthread_once(&once_control,once_routine);  //只调用一次，保证调用一次创建key 
	tsd_t *value = (tsd_t*)malloc(sizeof(tsd_t));
	value->tid = pthread_self();
	value->str = (char*)arg;
	pthread_setspecific(key_tsd, value);//各个线程根据自己需要往key_tsd中填入不同的值，一键多值 
	printf("%s setspacific %p\n", (char*)arg, value); 
	 
	value = pthread_getspecific(key_tsd);
	printf("tid=0x%x str=%s\n", (int)value->tid, value->str);
	sleep(2);
	value = pthread_getspecific(key_tsd);
	printf("tid=0x%x str=%s\n", (int)value->tid, value->str);
	return NULL;
}


int main()
{
	/*pthread_key_create(&key_tsd, destroy_routine); */ //每个线程都具有一个特定数据 
	                                                //那么当每个线程结束时，系统将调用这个函数来释放绑定在这个键上的内存块
	
	
	pthread_t tid1;
	pthread_t tid2;
	pthread_create(&tid1, NULL, thread_routine, "thread1");
	pthread_create(&tid2, NULL, thread_routine, "thread2");
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	pthread_key_delete(key_tsd);
	return 0;
} 