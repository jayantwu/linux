#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h> 
#include<signal.h>

void handler(int signum)
{
	printf("recv a sig=%d\n, child close\n", signum);
	exit(EXIT_SUCCESS);
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
	if((conn = accept(listenfd, (struct sockaddr*)&peeraddr, &peerlen))<0)
	{
		perror("accept fail\n");
		return -1;
	}
	
	printf("client ip :%s; client port: %d\n", inet_ntoa(peeraddr.sin_addr),
	       ntohs(peeraddr.sin_port));
	pid_t pid;
	pid = fork();   //创建子进程
	if (pid == -1)
		perror("fork fail\n");
	if (pid == 0)
	{//发送进程  ，子进程处理发送（键盘输入）  
		
		signal(SIGUSR1, handler);  //子进程收到信号后退出 
		char sendbuf[1024] = {0};
		while(fgets(sendbuf, sizeof(sendbuf), stdin)!= NULL)  //不停接收 
		{
			write(conn, sendbuf, strlen(sendbuf));
			memset(sendbuf, 0, sizeof(sendbuf));  //发送之后清空缓存，等待写入 
		} //循环退出的化，子进程结束 
		printf("child close\n"); 
		exit(EXIT_SUCCESS);
	}
	
	else
	{   //接收进程 ，父进程处理接受read，从socket读入数据，保存在recvbuf 
		char recvbuf[1024];
		while (1)
		{
			memset(recvbuf, 0, sizeof(recvbuf));//读之前清缓存 
			int ret = read(conn, recvbuf, sizeof(recvbuf));
			if (ret == -1)
			{
				perror("read fail\n");
				return -1;
			}
			if (ret == 0) 
			{
		        perror("client close\n");
			    break;
		     }
			printf("recv from client:%s", recvbuf);
		} 
		printf("parent close\n");
		kill(pid, SIGUSR1);  //向子进程发送信号，父进程退出时，子进程也退出 
		exit(EXIT_SUCCESS);
	}
	
	//关闭套接字 
	close(conn);
	close(listenfd);
	
	
}