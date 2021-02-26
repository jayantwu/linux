#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h> 
#include<signal.h>


ssize_t readn(int fd, void *buf, size_t count)
{
	size_t nleft = count;
	ssize_t nread;
	char * bufp = (char*)buf;
	while(nleft)
	{
		if((nread = read(fd, bufp, nleft))<0)
		{
			if (errno == EINTR)
				continue;
			return -1;
		}
		else if(nread == 0)
		{
			return count - nleft;
		}
		bufp += nread;
		nleft -= nread;
	}
	return count;
}
ssize_t writen(int fd, const void *buf, size_t count)
{
	size_t nleft = count;
	ssize_t nwrite;
	char * bufp = (char*)buf;
	while(nleft)
	{
		if((nwrite = write(fd, bufp, nleft))<0)
		{
			if (errno == EINTR)
				continue;
			return -1;
		}
		else if(nwrite == 0)
		{
			continue;
		}
		bufp += nwrite;
		nleft -= nwrite;
	}
	return count;
}

ssize_t recv_peek(int sockfd, void *buf, size_t len)
{
	while(1)
	{
		int ret = recv(sockfd, buf, len, MSG_PEEK);
		if (ret == -1 && errno == EINTR)
			continue;
		return ret;
	}
}

ssize_t readline(int sockfd, void *buf, size_t maxline)
{
	int ret;
	int nread; 
	char *bufp = (char*)buf;
	int nleft = maxline;
	while(1)
	{
		ret = recv_peek(sockfd, bufp, nleft);
		if (ret < 0)
			return ret;
		else if (ret == 0)
			return ret;
		nread = ret;
		int i;
		for (i = 0; i < nread; i++)
		{
			if (bufp[i] == '\n')
			{
				ret = readn(sockfd, bufp, i+1);//读走，包括'\n'
				if (ret != i+1)
					exit(EXIT_SUCCESS);
				return ret; 
			}
		}
		if (nread > nleft)
			exit(EXIT_SUCCESS);
		nleft -= nread;
		ret = readn(sockfd, bufp, nread);
		if (ret != nread)
			exit(EXIT_SUCCESS);
		bufp += nread;
	}
	return -1;
}


void do_service(int conn)
{
	char recvbuf[1024];
	int n;
	while (1)
	{
		memset(&recvbuf, 0, sizeof(recvbuf));
		int ret = readline(conn, recvbuf, 1024);
		if (ret == -1)
			perror("readline fail\n");
		if(ret ==0)
		{
			printf("client close\n");
		}
        
		printf("recv from client:%s", recvbuf);
		writen(conn, recvbuf, strlen(recvbuf));
	} 
	return;
}


int  main(int argc, char **argv)
{
	if (argc < 2)
	{
		printf("use \"server port\"\n");
		return -1;
	}
	
	int listenfd;
	short int port = atoi(argv[1]);
	if ((listenfd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP))< 0)
	/*if ((listenfd = socket(PF_INET, SOCK_STREAM, 0))< 0)*/  //第三个参数可以为0，前面两个参数实际已经决定是tcp了
	{
		perror("socket fail\n");
		return -1;
	}
	
	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	/*
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");  
	或者： 
	//int inet_aton(const char *cp, struct in_addr *inp);
	将地址转为网络序 
	inet_aton("127.0.0.1", (struct sockaddr*)&servaddr.sin_addr) 
	*/ 
	
	int on =1;
	if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
	{
		perror("setsockopt fail\n");
		return -1;
	}
	
	//绑定fd和ip端口 
	if (bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
	{
		perror("bind fail\n");
		return -1;
	} 
	//listenfd为被动套接字，能够接受连接 
	if (listen(listenfd, SOMAXCONN) < 0)  //已完成连接和未完成连接队列的和 队列的最大值 
	{
		perror("listen fail\n");
		return -1;
	} 
	
	struct sockaddr_in peeraddr;
	socklen_t peerlen = sizeof(peeraddr);   //peerlen一定要初始化 
	int conn;  //accept 返回一个连接 ，新的套接字，主动套接字，可以用来发送请求 
	
	pid_t pid;
	while(1)
	{   
	    //signal(SIGUSR1, handler);
		//连接socket 用于通信 
		if((conn = accept(listenfd, (struct sockaddr*)&peeraddr, &peerlen))<0)
	    {
		    perror("accept fail\n");
		    return -1;
	    }
	    printf("client ip :%s; client port: %d\n", inet_ntoa(peeraddr.sin_addr),
	       ntohs(peeraddr.sin_port));
	       
        pid = fork();
        if (pid == -1)
        {
        	perror("fork fail\n");
        	return -1;
        }
        
        if (pid == 0)  //子进程
		{
			//cnt++;
			//printf("client + 1\n");
			close(listenfd);
			do_service(conn);
			//kill(getppid(), SIGUSR1); 
			exit(0);
		} 
		if (pid > 0)
		{
			
			close(conn);//父进程不需要连接socket 
		}
	} 
	
	
	return 0;
	
	
}