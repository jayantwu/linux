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
	shm_unlink("/xyz"); 
	return 0;
} 