#include <unistd.h>
#include <sys/types.h>
#include <pthread.h> 
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define ERR_EXIT(m) \
	do { \
		perror(m); \
		exit(EXIT_FAILURE); \
	} while(0)
	
#define CONSUMERS_COUNT 1
#define PRODUCERS_COUNT 5
#define BUFFSIZE 10

int g_buffer[BUFFSIZE];

unsigned short in = 0;//从0号位置开始存放 
unsigned short out = 0;//从0号位置开始取走
unsigned short produce_id = 0;  //正在生产的id 
unsigned short consumer_id = 0;  //正在消费的id 

sem_t g_sem_full;
sem_t g_sem_empty;
pthread_mutex_t g_mutex;

pthread_t g_thread[CONSUMERS_COUNT + PRODUCERS_COUNT];  //存放线程id 

void* consume(void *arg) {
	int num = (long)arg;
	int i;
	while(1) {
		printf("%d wait buffer not empty\n", num);
		sem_wait(&g_sem_empty);  //开始消费时，已有的产品数减1 
		pthread_mutex_lock(&g_mutex);
		for(i = 0; i < BUFFSIZE; ++i) {
			printf("%02d ", i);  // 输出位宽为二 左边补0 
			if(g_buffer[i] == -1)
				printf("%s", "null");
			else
				printf("%d", g_buffer[i]);
			if(i == out)
				printf("\t<--consume");
			printf("\n");
		}
		int consume_id = g_buffer[out];
		printf("%d begin consume product %d\n", num, consume_id);
		g_buffer[out] = -1;	
		out = (out + 1) % BUFFSIZE;
		printf("%d end consume product %d\n", num, consume_id);
		pthread_mutex_unlock(&g_mutex);
		sem_post(&g_sem_full);  //消费结束时，能生产的数目加1	
		sleep(5);	
	}

	return NULL;
}

void* produce(void *arg) {
	int num = (long)arg;  //将void指针转回long 
	int i;
	while(1) {
		printf("%d wait buffer not full\n", num);
		sem_wait(&g_sem_full);//开始生产时，减1 （还能生产的个数） 
		pthread_mutex_lock(&g_mutex);   //加锁的时候 消费者无法消费 
		for(i = 0; i < BUFFSIZE; ++i) {
			printf("%02d ", i);
			if(g_buffer[i] == -1)
				printf("%s", "null");
			else
				printf("%d", g_buffer[i]);
			if(i == in)
				printf("\t<--produce");
			printf("\n");
		}
		
		printf("%d begin produce product %d\n", num, produce_id);
		g_buffer[in] = produce_id;	//产品id放进buffer 
		in = (in + 1) % BUFFSIZE;   //环形数组 ，下一个能存放的buffer中的空位置 
		printf("%d end produce product %d\n", num, produce_id++);  //produce_id+1 下一个 需要生产的产品id 
		pthread_mutex_unlock(&g_mutex);
		sem_post(&g_sem_empty);	//生产完时，empty+1,表示buffer多了一个产品 
		sleep(1);	
	}

	return NULL;
}

int main() {
	int i;
	for(i = 0; i < BUFFSIZE; ++i)   //buffer初始值为-1，表示没有产品 
		g_buffer[i] = -1;
	//初始化信号量和互斥锁 
	sem_init(&g_sem_full, 0, BUFFSIZE);   //第二个参数0表示线程间的信号量 
	sem_init(&g_sem_empty, 0, 0);
	
	pthread_mutex_init(&g_mutex, NULL);
	//创建消费者线程 
	for(i = 0; i < CONSUMERS_COUNT; ++i)
		pthread_create(&g_thread[i], NULL, consume, (void*)(long)i);  //需要传值，并强制转换为void指针 
    //创建生产者线程 
	for(i = 0; i < PRODUCERS_COUNT; ++i)
		pthread_create(&g_thread[CONSUMERS_COUNT + i], NULL, produce, (void*)(long)i);	
	//等待线程退出	
	for(i = 0; i < CONSUMERS_COUNT + PRODUCERS_COUNT; ++i)
		pthread_join(g_thread[i], NULL);	
	//销毁信号量和互斥锁 
	sem_destroy(&g_sem_full);
	sem_destroy(&g_sem_empty);
	pthread_mutex_destroy(&g_mutex);

	return 0;
}