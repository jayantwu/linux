#include<unistd.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<mqueue.h>
#include<string.h>

#include<stdlib.h>
#include<stdio.h>
#include<errno.h>

#define ERR_EXIT(m)\
	do\
	{\
		perror(m);\
		exit(EXIT_FAILURE);\		
	}while(1)
	
	
/*
	struct mq_attr {
               long mq_flags;       // Flags: 0 or O_NONBLOCK 
               long mq_maxmsg;      // Max. # of messages on queue 
               long mq_msgsize;     // Max. message size (bytes) 
               long mq_curmsgs;     //# of messages currently in queue 
           };
*/
typedef struct stu
{
	char name[32];
	int age;
}STU;

int main(int argc, char *argv[])
{
		
	mqd_t mqid;
	mqid = mq_open("/abc", O_RDONLY);
	if (mqid == (mqd_t)-1)
	{
		ERR_EXIT("mq_open");
	}
	
	struct mq_attr attr;
	mq_getattr(mqid, &attr);
	size_t size = attr.mq_msgsize;
    STU stu;
    unsigned prio;
	if(mq_receive(mqid, (char*)&stu, size, &prio)==-1)
	{
		ERR_EXIT("mq_receive");
	}
	printf("name=%s age=%d prio=%u\n", stu.name, stu.age, prio);
	mq_close(mqid);
	return 0;
} 
