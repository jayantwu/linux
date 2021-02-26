#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h> 



ssize_t readn(int fd, void *buf, size_t count)
{
	size_t nleft = count;  //剩余需要读的字节数 
	ssize_t nread;   //每次读的字节数 
	char * bufp = (char*)buf;
	while(nleft)//非0时一直循环读 
	{
		if((nread = read(fd, bufp, nleft))<0)
		{
			if (errno == EINTR)
				continue;
			return -1;
		}
		else if(nread == 0)
		{
			return count - nleft; //返回读了多少 
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

//偷窥的方式读取 
ssize_t recv_peek(int sockfd, void *buf, size_t len)
{
	while(1)
	{
		int ret = recv(sockfd, buf, len, MSG_PEEK);
		if (ret == -1 && errno == EINTR)  //返回失败或异常中断 
			continue;
		return ret;
	}
}
//可处理粘包  读一行，遇到'\n'才返回 
ssize_t readline(int sockfd, void *buf, size_t maxline)
{
	int ret;
	int nread; //已经读的字节数 
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
		//没有遇到'\n' ，需要继续读 
		if (nread > nleft)//处理异常 
			exit(EXIT_SUCCESS);
		nleft -= nread;
		ret = readn(sockfd, bufp, nread);//全部读到buff里 
		if (ret != nread)
			exit(EXIT_SUCCESS);
		bufp += nread;  //指针更新 
	}
	return -1;
}


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
	char sendbuf[1024] = {0};
	char recvbuf[1024] = {0};
	memset(sendbuf, 0, sizeof(sendbuf));
	memset(recvbuf, 0, sizeof(recvbuf));
	while (fgets(sendbuf, sizeof(sendbuf), stdin) != NULL)
	{
		
		writen(sock, sendbuf, strlen(sendbuf));  //发送 
		
		int ret = readline(sock, recvbuf, sizeof(recvbuf)); 
	
		if (ret == -1)  //读失败 
			perror("readline fail\n");

		if (ret == 0)
		{
			printf("server close\n");
			break;
		} 
		
		printf("recv from server: ");
		fputs(recvbuf, stdout);
		memset(sendbuf, 0, sizeof(sendbuf));//要清掉，否则可能有上一次的垃圾值 
		memset(recvbuf, 0, sizeof(recvbuf));
	} 
	//关闭套接字 
	close(sock);
	
	
}