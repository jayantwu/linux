// implementation of tcp client
#include "tcpclient.h"

bool tcpclient::connect(const string & ip, const uint16_t port)
{
    
    if (m_clientfd != -1) 
        return false;

    m_ip = ip;
    m_port = port;
    // step 1, create client socket
    m_clientfd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_clientfd == -1) {
        perror("socket");
        return false;
    }

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(m_port);
    struct hostent * h;
    if ((h = gethostbyname(m_ip.c_str())) == nullptr) {
        close(m_clientfd);
        m_clientfd = -1;
        return false;
    }
    memcpy(&servaddr.sin_addr, h->h_addr, h->h_length); // h->h_addr is big endian

    // connet to server
    if (::connect(m_clientfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
        close(m_clientfd);
        m_clientfd = -1;
        return false;
    }

    return true;

}