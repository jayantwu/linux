#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h> 

struct packet
{
	int len;
	char buf[1024];
};

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
			return count - nleft; //返回多少没读的 
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
	struct packet sendbuf;
	struct packet recvbuf;
	memset(&sendbuf, 0, sizeof(sendbuf));
	memset(&recvbuf, 0, sizeof(recvbuf));
    int n;
	while (fgets(sendbuf.buf, sizeof(sendbuf.buf), stdin) != NULL)
	{
		n = strlen(sendbuf.buf);
		sendbuf.len= htonl(n);  //转成网络序再发送 
		write(sock, &sendbuf, 4+n);  //发送 
		
		int ret = readn(sock, &recvbuf.len, 4);//先读4字节 
		//fputs(recvbuf, stdout);
		if (ret == -1)  //读失败 
			perror("read fail\n");
		if (ret < 4)  //没读完 
		{
			printf("server close\n");
			break;
		} 
        n = ntohl(recvbuf.len);  //4字节读成功 ，获取长度并开始读报文体 
        ret = readn(sock, &recvbuf.buf, n);
        if (ret == -1)
			perror("read fail\n");
		if (ret < n)
		{
			printf("server close\n");
			break;
		} 
		
		printf("recv from server: ");
		fputs(recvbuf.buf, stdout);
		memset(&sendbuf, 0, sizeof(sendbuf));//要清掉，否则可能有上一次的垃圾值 
		memset(&recvbuf, 0, sizeof(recvbuf));
	} 
	//关闭套接字 
	close(sock);
	
	
}