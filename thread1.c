#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>


void showa()
{
	int i;
	for(i=0; i <= 5; i++)
	{
		printf("aaa\n");
		sleep(1);
	}
		
} 

void showb()
{
	int i;
	for(i=0; i <= 5; i++)
	{
		printf("bbb\n");
		sleep(1);
	}
		
}


int main()
{
	pthread_t p1;
	pthread_t p2;
	pthread_create(&p1, NULL, (void*)showa, NULL);
	pthread_create(&p2, NULL, (void*)showb, NULL);
	
	sleep(1);
	pthread_join(p1, NULL);
	pthread_join(p1, NULL);
	printf("ccc\n");
	
}