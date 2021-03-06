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
	
    mq_notify(mqid, &sigev);   //����ע�ᣬ��Ҫ������Ϣ���ж�����Ϣ֮ǰ 
	STU stu;
    unsigned prio;
	if(mq_receive(mqid, (char*)&stu, size, &prio)==-1)  //����Ҫָ��ÿ����Ϣ�����ֵ 
	{
		ERR_EXIT("mq_receive");
	}
	printf("name=%s age=%d prio=%u\n", stu.name, stu.age, prio);
}

int main(int argc, char *argv[])
{
		
	
	mqid = mq_open("/abc", O_RDONLY);  //�ļ�Ҫ��/��ͷ 
	if (mqid == (mqd_t)-1)  //-1ǿ��ת����mqd_t 
	{
		ERR_EXIT("mq_open");
	}
	struct mq_attr attr;
	mq_getattr(mqid, &attr);
	size = attr.mq_msgsize;
	
	
	signal(SIGUSR1, handle_sigusr1); 
	
	
	sigev.sigev_notify = SIGEV_SIGNAL;
	sigev.sigev_signo = SIGUSR1;
	mq_notify(mqid, &sigev);  //�ӿն��е�����Ϣ���ܻ��֪ͨ�������ź�֮��᳷��ע�� 
	
	for(;;)   //Ҫ�Ӹ���ѭ�� ����֪ͨ���˾�û�л������ 
		pause();
		 
	mq_close(mqid);
	return 0;
} 
