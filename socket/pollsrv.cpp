#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <poll.h>
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

ssize_t readn(int fd, void *buf, size_t count) {//ssize_t �з�������  size_t �޷������� 
	size_t nleft = count;//ʣ����ֽ��� 
	ssize_t nread;//�ѽ��յ��ֽ��� 
	char *bufp = (char*)buf;
	
	while(nleft > 0) {
		if((nread = read(fd, bufp, nleft)) < 0) {
			if(errno == EINTR)//���ź��ж� 
				continue;
			return -1;
		}
		else if(nread == 0)//�Եȷ��ر��� 
			return count - nleft;
		bufp += nread; 
		nleft -= nread;
	}
	return count;
}

ssize_t writen(int fd, const void *buf, size_t count) {
	size_t nleft = count;//ʣ��Ҫ���͵��ֽ��� 
	ssize_t nwritten;//�ѷ��͵��ֽ��� 
	char *bufp = (char*)buf;
	
	while(nleft > 0) {
		if((nwritten = write(fd, bufp, nleft)) < 0) {
			if(errno == EINTR)//���ź��ж� 
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

ssize_t recv_peek(int sockfd, void *buf, size_t len) {//�������ݺ󲻽����ݴӻ������Ƴ� 
	while(1) {
		int ret = recv(sockfd, buf, len, MSG_PEEK);
		if(ret == -1 && errno == EINTR)//���ź��ж�
			continue;
		return ret;
	} 
}

ssize_t readline(int sockfd, void *buf, size_t maxline) {//ֻ�������׽ӿ� 
	int ret;
	int nread;
	char *bufp = (char *)buf;
	int nleft = maxline;
	while(1) {
		ret = recv_peek(sockfd, bufp, nleft);
		if(ret < 0)//�����ٽ����ж��жϣ���Ϊrecv_peek�����ڲ��Ѿ������� 
			return ret;
		else if(ret == 0)//�Է��ر�
			return ret; 
		nread = ret;
		int i;//�ж��ѽ��յ��Ļ��������Ƿ���\n 
		for(i = 0; i < nread; ++i) {
			if(bufp[i] == '\n') {
				ret = readn(sockfd, bufp, i + 1);//�����ݴӻ������Ƴ� 
				if(ret != i + 1) 
					exit(EXIT_FAILURE);
				return ret;
			}
		}
		
		if(nread > nleft)
			exit(EXIT_FAILURE);
		
		nleft -= nread;
		ret = readn(sockfd, bufp, nread);//��û����\n������Ҳ�ӻ������Ƴ�
		if(ret != nread) 
			exit(EXIT_FAILURE);
			
		bufp += nread;
	}
	return -1;
}

void echo_srv(int conn) {
	//����
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
	//�����ӽ��̵ĳ�ʼ״̬
	//wait(NULL); 
	while(waitpid(-1, NULL, WNOHANG) > 0);//���Եȴ������ӽ��̣�����0��ʾ�ȴ�����һ���ӽ��� 
}

void handle_sigpipe(int sig) {
	printf("recv a sig = %d\n", sig);
}

int main() {
	//signal(SIGPIPE, SIG_IGN);
	signal(SIGPIPE, handle_sigpipe);
	//signal(SIGCHLD, SIG_IGN);
	signal(SIGCHLD, handle_sigchld);
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
	socklen_t peerlen;
	int conn;
	
	/*
	struct pollfd {
		int   fd;//�ļ�������
		short events;//�����¼�/����Ȥ�¼� 
		short revents;//�����¼� 
	};
	*/
    //����ṹ�屣����һ��������Ϣ ��һ���ļ������� 
	int i; 
	struct pollfd client[2048];//�������ͻ���������Ϣ
	int maxi = 0;//���Ĳ����е�λ�� 
	
	for(i = 0; i < 2048; ++i)
		client[i].fd = -1;//-1��ʾ���е� 
	 
	int nready;//��⵽���¼����� 
	client[0].fd = listenfd;
	client[0].events = POLLIN;//�Կɶ��¼�����Ȥ ����ʾ���Ŀɶ��¼� 
	
	while(1) {
		nready = poll(client, maxi + 1, -1);    //int poll(struct pollfd *fds, nfds_t nfds, int timeout); ��һ��������һ���ṹ������ nfdsӦ��ʱ�ڲ�����ѭ��������
		if(nready == -1) {
			if(errno == EINTR)//���ź��ж�
				continue;
			ERR_EXIT("poll");
		}
		if(nready == 0)//��ʱ������timeout����ΪNULL���ʲ����ܷ���
			continue; 
		//�������׽��ֲ����˿ɶ��¼�ʱ 
		if(client[0].revents & POLLIN) {  //������λͼ ������ ���ĸ��¼�����ʱ���ͻ����client[i]�ṹ���revent��Ա��ֵ 
			peerlen = sizeof(peeraddr);//��������һ����ʼֵ 
			conn = accept(listenfd, (struct sockaddr*)&peeraddr, &peerlen);  //peeraddr�ṹ����Ϊ���� 
			if(conn == -1)
				ERR_EXIT("accept"); 
			//���浽ĳ������λ�� 
			for(i = 0; i < 2048; ++i) {
				if(client[i].fd < 0) {
					client[i].fd = conn;
					if(i > maxi)
						maxi = i;
					break;
				}		
			} 
			if(i == 2048) {
				fprintf(stderr, "too mang clients\n");
				exit(EXIT_FAILURE);
			}
			printf("ip=%s port=%d\n", inet_ntoa(peeraddr.sin_addr), ntohs(peeraddr.sin_port));//��ӡ���ӹ����Ŀͻ��˵ĵ�ַ��Ϣ 
			
			client[i].events = POLLIN;   //������� ���ӵĿɶ��¼� 
			
			if(--nready <= 0)//��⵽���¼��Ѿ��������ˣ�Ӧ����������û��Ҫ��������Ĵ��� 
				continue;	
		}
		
		//�������׽ӿڲ���������
		for(i = 1; i <= maxi; ++i) {
			conn = client[i].fd;
			if(conn == -1)
				continue;
			if(client[i].events & POLLIN) {
				char recvbuf[1024] = {0};
				int ret = readline(conn, recvbuf, 1024);//���ʱ��readline���ᱻ��������Ϊ�Ѿ��пɶ��¼������� 
				if(ret == -1)
					ERR_EXIT("readline");
				if(ret == 0) {
					printf("client close\n");
					client[i].fd = -1;
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