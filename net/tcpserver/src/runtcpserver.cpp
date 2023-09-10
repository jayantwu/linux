#include "tcpserver.h"
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
    if (argc != 2) {
        cout << "using ./tcpserver port" <<  endl;
        return -1;
    }
    
    int listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd == -1) {
        perror("socket");
        return -1;
    }

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(atoi(argv[1]));


    // bind ip and port
    if (bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) != 0) {
        perror("bind");
        close(listenfd);
        return -1;
    }

    struct sockaddr_in peer_addr;
    socklen_t peerlen = sizeof(peer_addr);
    int clientfd = accept(listenfd, (struct sockaddr*)(&peer_addr), &peerlen);
    if (clientfd == -1) {
        perror("accept");
        close(clientfd);
        return -1;
    }

    cout << "client connected." << endl;

    char buffer[1024];
    while (true) {
        int iret;
        memset(buffer, 0, sizeof(buffer));

        if ((iret = recv(clientfd, buffer, sizeof(buffer), 0)) <= 0) {
            cout << "iret=" << iret << endl;
            break;
        }

        cout << "recv: " << buffer << endl;

        strcpy(buffer, "server recv ok!");

        if ((iret = send(clientfd, buffer, strlen(buffer), 0)) <= 0) {
            perror("send");
            break;
        }

        cout << "send: " << buffer;

    }

    close(listenfd);
    close(clientfd);

    return 0;



    return 0;
}