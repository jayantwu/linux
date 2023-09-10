#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <cstring>
#include <iostream>


#define MAX_EVENTS 10

int main()
{
    int epoll_fd, nfds;
    struct epoll_event events[MAX_EVENTS];

    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        perror("Failed to create epoll instance");
        exit(EXIT_FAILURE);
    }

    int fd1 = open("file1.txt", O_WRONLY);
    int fd2 = open("file2.txt", O_RDONLY);

    struct epoll_event ev;
    std::memset(&ev, 0, sizeof(ev));
    ev.events = EPOLLIN; // 关注读事件
    ev.data.fd = fd1;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd1, &ev)) {
        perror("Failed to add fd1 to epoll instance");
        exit(EXIT_FAILURE);
    }

    // std::memset(&ev, 0, sizeof(ev));
    // ev.events = EPOLLIN; // 关注读事件
    // ev.data.fd = fd2;
    // if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd2, &ev)) {
    //     perror("Failed to add fd2 to epoll instance");
    //     exit(EXIT_FAILURE);
    // }

    while(1) {
        nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (nfds == -1) {
            perror("epoll wait");
            exit(EXIT_FAILURE);
        }
        std::cout << "nfds: " << nfds << std::endl;
        for (int i = 0; i < nfds; i++) {
            if (events[i].events & EPOLLIN) {
                printf("Read event on fd%d\n", events[i].data.fd);
            }
        }
    }

    close(fd1);
    close(fd2);
    close(epoll_fd);

    return 0;

}