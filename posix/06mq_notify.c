#include<unistd.h>
#include<sys/types.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<mqueue.h>
#include<string.h>
#include<signal.h>

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

size_t size;
mqd_t mqid;
struct sigevent sigev;

void handle_sigusr1(int sig)
{
	
    mq_notify(mqid, &sigev);   //重新注册，需要放在消息队列读出消息之前 
	STU stu;
    unsigned prio;
	if(mq_receive(mqid, (char*)&stu, size, &prio)==-1)  //长度要指定每条消息的最大值 
	{
		ERR_EXIT("mq_receive");
	}
	printf("name=%s age=%d prio=%u\n", stu.name, stu.age, prio);
}

int main(int argc, char *argv[])
{
		
	
	mqid = mq_open("/abc", O_RDONLY);  //文件要已/开头 
	if (mqid == (mqd_t)-1)  //-1强制转换成mqd_t 
	{
		ERR_EXIT("mq_open");
	}
	struct mq_attr attr;
	mq_getattr(mqid, &attr);
	size = attr.mq_msgsize;
	
	
	signal(SIGUSR1, handle_sigusr1); 
	
	
	sigev.sigev_notify = SIGEV_SIGNAL;
	sigev.sigev_signo = SIGUSR1;
	mq_notify(mqid, &sigev);  //从空队列到有消息才能获得通知，发送信号之后会撤销注册 
	
	for(;;)   //要加个死循环 否则通知到了就没有机会接受 
		pause();
		 
	mq_close(mqid);
	return 0;
} 
