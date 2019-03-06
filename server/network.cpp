/*
 * Author: Chengxiang
 * Date: 2019-2-21
 * implement of functions in network.h
*/

#include "network.h"
/*
 * @InputParam1 ip_address 
 * @InputParam2 port
 * @OutputParam1 sockservfd
 * 根据传入的ip_address和port完成socket的bind和listen,sockservfd返回服务端的socket描述符
*/
int Connect(const char *ip_address, int port, int *sockservfd)
{
    if((ip_address == NULL) || (port <= 0))
    {
        printf("input param error!\n");
        return -1;
    }

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, ip_address, &servaddr.sin_addr);
    servaddr.sin_port = htons(port);

    *sockservfd = socket(AF_INET, SOCK_STREAM, 0);
    if(*sockservfd <= 0)
    {
        printf("socket error!\n");
        return -1;
    }

    if(bind(*sockservfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
    {
        printf("bind error!\n");
        return -1;
    }

    if(listen(*sockservfd, 100) < 0)
    {
        printf("listen error!\n");
        return -1;
    }

    return 0;
}

int setnonblocking(int fd)
{
    int oldopt = fcntl(fd, F_GETFL);
    int newopt = oldopt | NONBLOCK;
    fcntl(fd, F_SETFL, newopt);
    return oldopt;
}

//向epollfd中添加事件
void addfd(int epollfd, int fd)
{
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET;
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    setnonblocking(fd);
}

void sig_handler(int sig, int pipefd[])
{
    int save_errno = errno;
    int msg = sig;
    send(pipefd[1], (char*)&msg, 1, 0); //通过管道通知主循环
    errno = save_errno;
}

void addsig(int sig)
{
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = sig_handler;
    sa.sa_flags |= SA_RESTART;
    sigfillset(&sa.mask);
    if(sigaction(sig, &sa, NULL) < 0)
    {
        printf(stderr, "sigaction error!");
        return ;
    }
}