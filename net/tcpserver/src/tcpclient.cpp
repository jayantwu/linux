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
        ::close(m_clientfd);
        m_clientfd = -1;
        return false;
    }
    memcpy(&servaddr.sin_addr, h->h_addr, h->h_length); // h->h_addr is big endian

    // connet to server
    if (::connect(m_clientfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
        ::close(m_clientfd);
        m_clientfd = -1;
        return false;
    }

    return true;

}

bool tcpclient::send(const string & buffer)
{
    if (m_clientfd == -1) return false;

    if ((::send(m_clientfd, buffer.data(), buffer.size(), 0)) <= 0)
        return false;

    return true;    
}

bool tcpclient::recv(string & buffer, const size_t maxlen)
{
    buffer.clear();
    buffer.resize(maxlen);
    int readn = ::recv(m_clientfd, &buffer[0], buffer.size(), 0);
    if (readn <= 0) {
        buffer.clear();
        return false;
    }
    buffer.resize(readn); // resize buffer size
    return true;
}

bool tcpclient::close()
{
    if (m_clientfd == -1) {
        return false;
    }

    ::close(m_clientfd);
    m_clientfd = -1;
    return true;
}