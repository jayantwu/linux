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
	
#define CONSUMERS_COUNT 2
#define PRODUCERS_COUNT 1

pthread_mutex_t g_mutex;
pthread_cond_t g_cond;

pthread_t g_thread[CONSUMERS_COUNT + PRODUCERS_COUNT];

int nready = 0;

void* consume(void *arg) {
	int num = (long)arg;
	while(1) {
		pthread_mutex_lock(&g_mutex);   //遇到已加锁时，阻塞再这里 
		while(nready == 0) {//条件不满足则等待 
			//解锁，等待其他线程改变nready的值 
			printf("consumer %d begin wait a condition...\n", num);
			pthread_cond_wait(&g_cond, &g_mutex);   //解锁之后哪个线程先加锁？wait收到信号之后要重新加锁 ，并返回 
		}
		printf("consumer %d end wait a condition...\n", num);
        printf("consumer %d begin consume product...\n", num);
		--nready;
		pthread_mutex_unlock(&g_mutex);
		sleep(1);
	}

	return NULL;
}

void* produce(void *arg) {
	int num = (long)arg;

	while(1) {
		pthread_mutex_lock(&g_mutex);
        printf("producer %d begin produce product...\n", num);
		++nready;
        printf("producer %d end produce product...\n", num);
		printf("producer %d signal...\n", num);
		pthread_cond_signal(&g_cond);  //激活线程 ,解锁，发通知，但是中间过程中可能有其他消费者先加锁（因为是非原子操作），并消费了，导致当前消费者被唤醒之后仍然不满足条件（虚假唤醒）
		pthread_mutex_unlock(&g_mutex);
		sleep(5);
	}

	return NULL;
}

int main() {
	int i;
	
	pthread_mutex_init(&g_mutex, NULL);
	pthread_cond_init(&g_cond, NULL);  //初始化条件变量 
	

	for(i = 0; i < CONSUMERS_COUNT; ++i)
		pthread_create(&g_thread[i], NULL, consume, (void*)(long)i);
		
	//sleep(1);   //消费者创建出来sleep 1秒再创建生产者，保证能进入等待状态 

	for(i = 0; i < PRODUCERS_COUNT; ++i)
		pthread_create(&g_thread[CONSUMERS_COUNT + i], NULL, produce, (void*)(long)i);	
		
	for(i = 0; i < CONSUMERS_COUNT + PRODUCERS_COUNT; ++i)
		pthread_join(g_thread[i], NULL);	
		
	pthread_mutex_destroy(&g_mutex);
	pthread_cond_destroy(&g_cond);

	return 0;
}


/* 
@localhost thread]$ ./pctest2
consumer 1 begin wait a condition...    //消费者1进入等待状态并解锁 
producer 0 begin produce product...     //生产者开始生产 
producer 0 end produce product...
producer 0 signal...
consumer 0 end wait a condition...      //消费者0抢到了锁，条件满足开始消费 
consumer 0 begin consume product...     // 
consumer 1 begin wait a condition...    //消费者1的wait函数返回发现条件还是没满足又进入了的等待状态 ，释放了锁 
consumer 0 begin wait a condition...    //消费者0加锁但是条件不满足，进入等待状态 并解锁 
producer 0 begin produce product...     //生产者加锁生产 
producer 0 end produce product...
producer 0 signal...
consumer 1 end wait a condition...      //消费者1再线程等待队列前面，首先 获得锁进行消费 
consumer 1 begin consume product...
consumer 1 begin wait a condition...    //消费完之后再次进入等待状态，解锁 
producer 0 begin produce product...     //生产者生产 
producer 0 end produce product...
producer 0 signal...
consumer 0 end wait a condition...
consumer 0 begin consume product...
consumer 0 begin wait a condition...
*/ 
