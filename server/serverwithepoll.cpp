/*
 * Author: Chengxiang
 * Date: 2019-2-21
 * 服务端IO复用，使用epoll
*/

#include "network.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <sys/epoll.h>

const int MAX_EVENTS = 16;

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("usage: %s ip_address port_number\n", basename(argv[0]));
        return -1;
    }

    const char *ip = argv[1];
    int port = atoi(argv[2]);
    assert(port > 0);

    struct sockaddr_in servsock, clisock;
    memset(&servsock, 0, sizeof(servsock));
    servsock.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &servsock.sin_addr);
    servsock.sin_port = htons(port);

    int sockserv = socket(AF_INET, SOCK_STREAM, 0);
    assert(sockserv > 0);

    if(bind(sockserv, (struct sockaddr*)&servsock, sizeof(servsock)) < 0)
    {
        printf("bind error!\n");
        return -1;
    }

    if(listen(sockserv, 100) < 0)
    {
        printf("listen error!\n");
        return -1;
    }

    struct epoll_event event, events[MAX_EVENTS];
    int epoll_fd = epoll_create1(0);
    if(epoll_fd < 0)
    {
        printf("epoll error!\n");
        return -1;
    }

    event.events = EPOLLIN | EPOLLET;
    event.data.fd = sockserv;
    if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sockserv, &event) != 0)
    {
        printf("epoll_ctl error!\n");
        close(epoll_fd);
        return -1;
    }

    //
    int event_count = 0;
    while(1)
    {
        event_count = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        printf("%d ready events\n", event_count);
        for(int i = 0; i < event_count; ++i)
        {
            int fd = events[i].data.fd;
            assert(fd > 0);
            if(fd == sockserv)//new connection
            {
                socklen_t clilen = sizeof(clisock);
                int sockcli = accept(fd, (struct sockaddr*)&clisock, &clilen);
                assert(sockcli > 0);
                // epoll_event event;
                event.data.fd = sockcli;
                event.events = EPOLLIN | EPOLLET;
                epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sockcli, &event);
            }
            else if(events[i].events & EPOLLIN)//existed(old) connection
            {
                char buff[1024];
                int ret = read(fd, buff, 1024);
                if(ret > 0)
                {
                    buff[ret] = '\0';
                    printf("REC %d characters: %s\n", ret, buff);
                    event.data.fd = fd;
                    event.events = EPOLLOUT | EPOLLET;
                    epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &event);
                }
                else
                {
                    printf("disconnection from %d\n", fd);
                    close(fd);
                    event.data.fd = fd;
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, &event);
                }
            }
            else if(events[i].events & EPOLLOUT)
            {
                const char *hello = "hello from server";
                write(fd, hello, strlen(hello));
                event.data.fd = fd;
                event.events = EPOLLIN | EPOLLET;
                epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &event);
            }
        }
    }

    if(close(epoll_fd) != 0)
    {
        printf("close epoll_fd error!\n");
        return -1;
    }

    return 0;
}