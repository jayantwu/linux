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
		pthread_mutex_lock(&g_mutex);   //�����Ѽ���ʱ������������ 
		while(nready == 0) {//������������ȴ� 
			//�������ȴ������̸߳ı�nready��ֵ 
			printf("consumer %d begin wait a condition...\n", num);
			pthread_cond_wait(&g_cond, &g_mutex);   //����֮���ĸ��߳��ȼ�����wait�յ��ź�֮��Ҫ���¼��� �������� 
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
		pthread_cond_signal(&g_cond);  //�����߳� 
		pthread_mutex_unlock(&g_mutex);
		sleep(5);
	}

	return NULL;
}

int main() {
	int i;
	
	pthread_mutex_init(&g_mutex, NULL);
	pthread_cond_init(&g_cond, NULL);  //��ʼ���������� 
	

	for(i = 0; i < CONSUMERS_COUNT; ++i)
		pthread_create(&g_thread[i], NULL, consume, (void*)(long)i);
		
	//sleep(1);   //�����ߴ�������sleep 1���ٴ��������ߣ���֤�ܽ���ȴ�״̬ 

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
consumer 1 begin wait a condition...    //������1����ȴ�״̬������ 
producer 0 begin produce product...     //�����߿�ʼ���� 
producer 0 end produce product...
producer 0 signal...
consumer 0 end wait a condition...      //������0�����������������㿪ʼ���� 
consumer 0 begin consume product...     // 
consumer 1 begin wait a condition...    //������1��wait�������ط�����������û�����ֽ����˵ĵȴ�״̬ ���ͷ����� 
consumer 0 begin wait a condition...    //������0�����������������㣬����ȴ�״̬ ������ 
producer 0 begin produce product...     //�����߼������� 
producer 0 end produce product...
producer 0 signal...
consumer 1 end wait a condition...      //������1���̵߳ȴ�����ǰ�棬���� ������������� 
consumer 1 begin consume product...
consumer 1 begin wait a condition...    //������֮���ٴν���ȴ�״̬������ 
producer 0 begin produce product...     //���������� 
producer 0 end produce product...
producer 0 signal...
consumer 0 end wait a condition...
consumer 0 begin consume product...
consumer 0 begin wait a condition...
*/ 