#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h> 
int  main(int argc, char **argv)
{
	if (argc < 3)
	{
		printf("use client 127.0.0.1 5188\n");
		return -1;
	}
	
	
	int sock;
	short int port = atoi(argv[2]);
	if ((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP))< 0)
	/*if ((listenfd = socket(PF_INET, SOCK_STREAM, 0))< 0)*/  //第三个参数可以为0，前面两个参数实际已经决定是tcp了
	{
		perror("socket fail\n");
		return -1;
	}
	
	struct sockaddr_in servaddr;
	memset(&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);

	//servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_addr.s_addr = inet_addr(argv[1]);
	if(connect(sock, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
	{
		perror("connect fail\n");
		return -1;
	}
	
	
	pid_t  pid;
	pid = fork();
	if (pid == -1)
		perror("fork fail\n");
	//parent send,  child recv
	if (pid > 0)
	{
		char recvbuf[1024] = {0};//初始化接收缓存 
		while(1)
		{
			memset(recvbuf, 0, sizeof(recvbuf));
			int ret = read(sock, recvbuf, sizeof(recvbuf));
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
		    fputs(recvbuf, stdout);
		}
		exit(EXIT_SUCCESS); 
	}
	if (pid == 0)
	{   
	    char sendbuf[1024] = {0};  //初始化发送缓存 
		while (fgets(sendbuf, sizeof(sendbuf), stdin) != NULL)
		{
			write(sock, sendbuf, strlen(sendbuf));
			memset(sendbuf, 0, sizeof(sendbuf));
		} 
		exit(EXIT_SUCCESS);
	}
	
	//关闭套接字 
	close(sock);

}