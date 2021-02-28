#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>
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

ssize_t readn(int fd, void *buf, size_t count) {//ssize_t 有符号整数  size_t 无符号整数 
	size_t nleft = count;//剩余的字节数 
	ssize_t nread;//已接收的字节数 
	char *bufp = (char*)buf;
	
	while(nleft > 0) {
		if((nread = read(fd, bufp, nleft)) < 0) {
			if(errno == EINTR)//被信号中断 
				continue;
			return -1;
		}
		else if(nread == 0)//对等方关闭了 
			return count - nleft;
		bufp += nread; 
		nleft -= nread;
	}
	return count;
}

ssize_t writen(int fd, const void *buf, size_t count) {
	size_t nleft = count;//剩余要发送的字节数 
	ssize_t nwritten;//已发送的字节数 
	char *bufp = (char*)buf;
	
	while(nleft > 0) {
		if((nwritten = write(fd, bufp, nleft)) < 0) {
			if(errno == EINTR)//被信号中断 
				continue;
			return -1;
		}
		else if(nwritten == 0)
			continue;
		bufp += nwritten; 
		nleft -= nwritten;
	}
	return count;	
}

ssize_t recv_peek(int sockfd, void *buf, size_t len) {//接收数据后不将数据从缓冲区移除 
	while(1) {
		int ret = recv(sockfd, buf, len, MSG_PEEK);
		if(ret == -1 && errno == EINTR)//被信号中断
			continue;
		return ret;
	} 
}

ssize_t readline(int sockfd, void *buf, size_t maxline) {//只能用于套接口 
	int ret;
	int nread;
	char *bufp = (char *)buf;
	int nleft = maxline;
	while(1) {
		ret = recv_peek(sockfd, bufp, nleft);
		if(ret < 0)//不用再进行中断判断，因为recv_peek函数内部已经进行了 
			return ret;
		else if(ret == 0)//对方关闭
			return ret; 
		nread = ret;
		int i;//判断已接收到的缓冲区中是否有\n 
		for(i = 0; i < nread; ++i) {
			if(bufp[i] == '\n') {
				ret = readn(sockfd, bufp, i + 1);//将数据从缓冲区移除 
				if(ret != i + 1) 
					exit(EXIT_FAILURE);
				return ret;
			}
		}
		
		if(nread > nleft)
			exit(EXIT_FAILURE);
		
		nleft -= nread;
		ret = readn(sockfd, bufp, nread);//还没遇到\n的数据也从缓冲区移除
		if(ret != nread) 
			exit(EXIT_FAILURE);
			
		bufp += nread;
	}
	return -1;
}

void echo_srv(int conn) {
	//接收
	char recvbuf[1024];
	while(1) {
		memset(recvbuf, 0, sizeof(recvbuf));
		int ret = readline(conn, recvbuf, 1024);
		if(ret == -1)
			ERR_EXIT("readline");
		if(ret == 0) {
			printf("client close\n");
			break;
		} 
		
		fputs(recvbuf, stdout);
		writen(conn, recvbuf, strlen(recvbuf));
	} 	
}

void handle_sigchld(int sig) {
	//捕获子进程的初始状态
	//wait(NULL); 
	while(waitpid(-1, NULL, WNOHANG) > 0);//可以等待所有子进程，大于0表示等待到了一个子进程 
}

int main() {
	//signal(SIGCHLD, SIG_IGN);
	signal(SIGCHLD, handle_sigchld);
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
	socklen_t peerlen;
	int conn;

	int i; 
	int client[FD_SETSIZE];//保存多个客户端连接信息， FD_SETSIZE一般等于单进程最大能打开的文件fd 
	int maxi = 0;//最大的不空闲的位置 
	
	for(i = 0; i < FD_SETSIZE; ++i)//初始化 
		client[i] = -1;//-1表示空闲的 
	 
	int nready;//检测到的事件个数 
	int maxfd = listenfd;//一开始只有监听fd 
	fd_set rset;  //定义读集合 
	fd_set allset;  
	FD_ZERO(&rset);  //初始化 
	FD_ZERO(&allset);
	FD_SET(listenfd, &allset); //将监听fd加入到集合 
	
	while(1) {
		rset = allset;
		nready = select(maxfd + 1, &rset, NULL, NULL, NULL);  //只关心读集合 
		if(nready == -1) {
			if(errno == EINTR)//被信号中断，继续处理 
				continue;
			ERR_EXIT("select");
		}
		if(nready == 0)//超时，现在timeout设置为NULL，故不可能发生
			continue; 
		if(FD_ISSET(listenfd, &rset))   //如果监听fd读事件发生，调用accpt 
		{
			peerlen = sizeof(peeraddr);//必须设置一个初始值 
			conn = accept(listenfd, (struct sockaddr*)&peeraddr, &peerlen);
			if(conn == -1)
				ERR_EXIT("accept"); 
			//保存到某个空闲位置，用于处理之后的读事件的发生 
			for(i = 0; i < FD_SETSIZE; ++i) 
			{
				if(client[i] < 0) {
					client[i] = conn;
					if(i > maxi)
						maxi = i;
					break;
				}		
			} 
			if(i == FD_SETSIZE) 
			{
				fprintf(stderr, "too mang clients\n");
				exit(EXIT_FAILURE);
			}
			printf("ip=%s port=%d\n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));
			
			FD_SET(conn, &allset);//将连接fd加入到集合 
			if(conn > maxfd)
				maxfd = conn;  //更新maxfd 
			
			if(--nready <= 0)//检测到的事件已经处理完了，应继续监听，没必要处理下面的代码 
				continue;	
		}
		 
		//已连接套接口产生了数据，需要遍历 
		for(i = 0; i <= maxi; ++i) {
			conn = client[i];
			if(conn == -1)  //该位置空闲，直接循环下一个位置 
				continue;
			if(FD_ISSET(conn, &rset))   //判断是不是该连接fd产生了读事件 ，调用read 
			{
				char recvbuf[1024] = {0};
				int ret = readline(conn, recvbuf, 1024);
				if(ret == -1)
					ERR_EXIT("readline");
				if(ret == 0) 
				{
					printf("client close\n");
					FD_CLR(conn, &allset);//对端关闭时，将这个连接移出集合 
					client[i] = -1;   //恢复成空闲 
                    close(conn);  
				} 
				
				fputs(recvbuf, stdout);
				writen(conn, recvbuf, strlen(recvbuf));
				
				if(--nready <= 0)
					break;
			}	
		}
	}
	return 0;
} 