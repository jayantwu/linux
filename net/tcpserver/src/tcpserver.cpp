// implementation of tcp server
#include "tcpserver.h"

bool tcpserver::init_server(const uint16_t port)
{
    // step 1 create socket
    if ((m_listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        return false;
    m_port = port;

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(m_port);

    // step 2
    if (bind(m_listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
        close(m_listenfd);
        m_listenfd = -1;
        return false;
    }

    // step 3
    if (listen(m_listenfd, 5) == -1) {
        close(m_listenfd);
        m_listenfd = -1;
        return false;
    }

    return true;
      
}

bool tcpserver::accept()
{
    struct sockaddr_in peer_addr;
    socklen_t peerlen = sizeof(peer_addr);
    m_clientfd = ::accept(m_listenfd, (struct sockaddr*)(&peer_addr), &peerlen);  // peer_addr will return
    if (m_clientfd == -1) {
        close(m_listenfd);
        m_listenfd = -1;
        return false;
    }

    m_clientip = inet_ntoa(peer_addr.sin_addr);

    return true;
}

const string& tcpserver::clientip() const
{
    return m_clientip;
}


bool tcpserver::send(const string &buffer)
{
    if (m_clientfd == -1) return false;

    if ((::send(m_clientfd, buffer.data(), buffer.size(), 0)) < 0) {
        return false;
    }

    return true;
}

bool tcpserver::recv(string &buffer, const size_t maxlen)
{
    buffer.clear();
    buffer.resize(maxlen);
    int readn = ::recv(m_clientfd, &buffer[0], buffer.size(), 0);
    if (readn <= 0) {
        buffer.clear();
        return false;
    }
    buffer.resize(readn);

    return true;
}

bool tcpserver::closelisten()
{
    if (m_listenfd == -1)
        return false;

    ::close(m_listenfd);
    m_listenfd = -1;
    return true;
}

bool tcpserver::closeclient()
{
    if (m_clientfd == -1)
        return false;

    ::close(m_clientfd);
    m_clientfd = -1;
    return true;
}