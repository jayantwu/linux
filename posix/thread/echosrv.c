#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <netdb.h> 

#define ERR_EXIT(m) \
	do { \
		perror(m); \
		exit(EXIT_FAILURE); \
	} while(0)

void do_service(int conn) {
	//����
	char recvbuf[1024];
	while(1) {
		memset(recvbuf, 0, sizeof(recvbuf));
		int ret = read(conn, recvbuf, sizeof(recvbuf));
		if(ret == 0) {
			printf("client close\n");
			break;
		} 
		else if(ret == -1)
			ERR_EXIT("read");
		fputs(recvbuf, stdout);
		write(conn, recvbuf, ret);
	} 	
}

void* thread_routine(void* arg) {
	pthread_detach(pthread_self());  //���߳������̷߳��룬��ֹ���߳̽���֮�������ʬ�߳� 
	int conn = (int)arg;
	free(arg);//ȡ��֮��ָ���ͷ� 
	do_service(conn);
	printf("exiting thread����\n");
	return NULL;
}

int main() {
	//�����׽��� 
	int listenfd;
	if((listenfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		ERR_EXIT("socket");
	
	//��ַ��ʼ��	
	struct sockaddr_in servaddr; //IPv4��ַ�ṹ
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(5188); 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//�󶨱����������ַ 
    
    int on = 1;
    if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
        ERR_EXIT("setsockopt");
	
	//��
	if(bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
		ERR_EXIT("bind");
		
	//����
	if(listen(listenfd, SOMAXCONN) < 0)
		ERR_EXIT("listen");
	
	//����	
	struct sockaddr_in peeraddr;
	socklen_t peerlen = sizeof(peeraddr);
	int conn;
	
	while(1) {
		if((conn = accept(listenfd, (struct sockaddr*)&peeraddr, &peerlen)) < 0)
			ERR_EXIT("accept"); 
		printf("ip=%s port=%d\n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));
		
		pthread_t tid;  //����һ�����߳������������׽��� 
		int ret;
		int *p = malloc(sizeof(int));   //�¿���һ���ڴ��ŵ�ǰ��conn 
		*p = conn;
		/*pthread_create(&tid, NULL, thread_routine, (void*)conn)*/     //����Ҫ��ֵ��conn�������߻���ھ�̬��race condition���� ����ȱ���ǲ�����ֲ��32Ϊ��ַΪ4�ֽڣ�64λϵͳλ8�ֽ� 
		/*pthread_create(&tid, NULL, thread_routine, (void*)&conn)*/     //���ִ���ַ�ķ�ʽ���ھ�̬ ��conn��ֵ���ܱ���һ�����Ӹĵ� 
		if((ret = pthread_create(&tid, NULL, thread_routine, p)) != 0) {
			fprintf(stderr, "pthread_create:%s\n", strerror(ret));
			exit(EXIT_FAILURE);
		}
	} 
	
	return 0;
} 