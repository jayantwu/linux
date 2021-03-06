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
	//接收
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
	pthread_detach(pthread_self());  //将线程与主线程分离，防止主线程结束之后产生僵尸线程 
	int conn = (int)arg;
	free(arg);//取完之后将指针释放 
	do_service(conn);
	printf("exiting thread……\n");
	return NULL;
}

int main() {
	//创建套接字 
	int listenfd;
	if((listenfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
		ERR_EXIT("socket");
	
	//地址初始化	
	struct sockaddr_in servaddr; //IPv4地址结构
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(5188); 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//绑定本机的任意地址 
    
    int on = 1;
    if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
        ERR_EXIT("setsockopt");
	
	//绑定
	if(bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
		ERR_EXIT("bind");
		
	//监听
	if(listen(listenfd, SOMAXCONN) < 0)
		ERR_EXIT("listen");
	
	//连接	
	struct sockaddr_in peeraddr;
	socklen_t peerlen = sizeof(peeraddr);
	int conn;
	
	while(1) {
		if((conn = accept(listenfd, (struct sockaddr*)&peeraddr, &peerlen)) < 0)
			ERR_EXIT("accept"); 
		printf("ip=%s port=%d\n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));
		
		pthread_t tid;  //创建一个子线程来处理连接套接字 
		int ret;
		int *p = malloc(sizeof(int));   //新开辟一个内存存放当前的conn 
		*p = conn;
		/*pthread_create(&tid, NULL, thread_routine, (void*)conn)*/     //这里要传值（conn），否者会存在竞态（race condition）， 但是缺点是不可移植，32为地址为4字节，64位系统位8字节 
		/*pthread_create(&tid, NULL, thread_routine, (void*)&conn)*/     //这种传地址的方式存在竞态 ，conn的值可能被下一个连接改掉 
		if((ret = pthread_create(&tid, NULL, thread_routine, p)) != 0) {
			fprintf(stderr, "pthread_create:%s\n", strerror(ret));
			exit(EXIT_FAILURE);
		}
	} 
	
	return 0;
} 