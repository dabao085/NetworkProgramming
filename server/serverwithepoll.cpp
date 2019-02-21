/*
 * Author: Chengxiang
 * Date: 2019-2-21
 * 服务端IO复用，使用epoll
*/

#include "network.h"
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
    int sockserv;
    if(Connect(ip, port, &sockserv) < 0)
    {
        printf("Connect error!\n");
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
                struct sockaddr_in cliaddr;
                socklen_t clilen = sizeof(cliaddr);
                int sockcli = accept(fd, (struct sockaddr*)&cliaddr, &clilen);
                assert(sockcli > 0);
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