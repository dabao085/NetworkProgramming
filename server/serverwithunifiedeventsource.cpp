/*
 * Author: Cheng xiang
 * Date: 2019-3-1
 * server实现统一事件源
*/

#include "network.h"
const int MAX_EPOLL = 32;
static int pipefd[2];

void sig_handler(int sig)
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
    sigfillset(&sa.sa_mask);
    if(sigaction(sig, &sa, NULL) < 0)
    {
        printf("sigaction error!");
        return ;
    }
}

int main(int argc, char *argv[])
{
    if(argc < 3)
    {
        printf("usage: %s ip_address port_number\n", basename(argv[0]));
        return -1;
    }

    const char *ip = argv[1];
    int port = atoi(argv[2]);
    assert(port > 0);

    int sockserv;
    if(Connect(ip, port, &sockserv) < 0)
    {
        printf("Connect error!\n");
        return -1;
    }

    struct epoll_event events[MAX_EPOLL];
    int epoll_fd = epoll_create1(0);
    if(epoll_fd < 0)
    {
        printf("epoll_create error!\n");
        return -1;
    }
    addfd(epoll_fd, sockserv);

    //创建全双工管道
    int ret = socketpair(AF_UNIX, SOCK_STREAM, 0, pipefd);
    if(ret < 0)
    {
        printf("socketpair error!\n");
        return -1;
    }
    setnonblocking(pipefd[1]);
    addfd(epoll_fd, pipefd[0]);

    //添加对信号的处理
    addsig(SIGHUP);
    addsig(SIGCHLD);
    addsig(SIGTERM);
    addsig(SIGINT);

    bool stop = false;
    while(!stop)
    {
        int number = epoll_wait(epoll_fd, events, MAX_EPOLL, -1);
        if(number < 0)
        {
            printf("epoll_wait error!\n");
            return 0;
        }

        for(int i = 0; i < number; ++i)
        {
            int sockfd = events[i].data.fd;
            if(sockfd == sockserv)
            {
                struct sockaddr_in client_addr;
                socklen_t clilen = sizeof(client_addr);
                int sockclient = accept(sockserv, (struct sockaddr*)&client_addr, &clilen);
                if(sockclient < 0)
                {
                    printf("accept error!\n");
                    return -1;
                }
                addfd(epoll_fd, sockclient);
            }
            //处理信号事件
            else if((sockfd == pipefd[0]) && (events[i].events & EPOLLIN))
            {
                char sig[1024];
                int ret = recv(sockfd, sig, sizeof(sig), 0);
                if(ret < 0)
                {
                    printf("recv error!\n");
                    return -1;
                }
                else if(ret == 0)
                {
                    continue;
                }
                else
                {
                    for(int i = 0; i < ret; ++i)
                    {
                        switch(sig[i])
                        {
                            case SIGCHLD:
                            case SIGHUP:
                            {
                                continue;
                            }
                            case SIGTERM:
                            case SIGINT:
                            {
                                stop = true;
                            }
                        }
                    }

                }
                
            }
            else
            {}
            
        }
    }
    printf("close fd\n");
    close(sockserv);
    close(pipefd[0]);
    close(pipefd[1]);

    return 0;
}