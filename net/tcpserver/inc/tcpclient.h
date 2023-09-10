#pragma once

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
class tcpclient {
    private:
        int m_clientfd;
        uint32_t m_port;
        string m_ip;

        
    public:
        tcpclient(){ 
            // todo
        }
        ~tcpclient(){
            //to do
        }

        bool connect(const string & ip, const uint16_t port);

        int get_client_fd() { return m_clientfd; }

};