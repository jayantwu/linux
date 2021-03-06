#include<unistd.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<mqueue.h>

#include<stdlib.h>
#include<stdio.h>
#include<errno.h>

#define ERR_EXIT(m)\
	do\
	{\
		perror(m);\
		exit(EXIT_FAILURE);\		
	}while(1)
	
	


int main()
{
	mqd_t mqid;
	mqid = mq_open("/abc", O_RDONLY);
	if (mqid == (mqd_t)-1)
	{
		ERR_EXIT("mq_open");
	}
	printf("creat mq success\n");
/*
	struct mq_attr {
               long mq_flags;       // Flags: 0 or O_NONBLOCK 
               long mq_maxmsg;      // Max. # of messages on queue 
               long mq_msgsize;     // Max. message size (bytes) 
               long mq_curmsgs;     //# of messages currently in queue 
           };
*/
	struct mq_attr attr;
	mq_getattr(mqid, &attr);
	printf("max #msg=%ld max #bytes/msg=%ld #currently on queue=%ld\n",
	attr.mq_maxmsg, attr.mq_msgsize, attr.mq_curmsgs);
	mq_close(mqid);
	return 0;
} 
