#pragma once
#include <cstring>
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
#include <netdb.h>
#include <sys/epoll.h>

#include <string>
using namespace std;

class tcpserver {
    private:
        int m_listenfd;
        int m_clientfd;
        string m_clientip;
        uint16_t m_port;
        
    public:
        tcpserver(): m_listenfd(-1), m_clientfd(-1) {}
        ~tcpserver() {
            closelisten();
            closeclient();
        }

        bool init_server(const uint16_t port);

        bool accept();
    
        const string& clientip() const;

        bool send(const string &buffer);

        bool recv(string &buffer, const size_t maxlen);

        bool closelisten();

        bool closeclient();

        int start();

        int stop();

};