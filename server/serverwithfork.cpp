/*
 * Author: Chengxiang
 * Date: 2019-2-19
 * 循环接收新连接，每接收一个就开一个进程来处理连接
*/

#include "network.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

//用于回收已完成子进程的信息，防止出现僵尸进程
void sig_chld(int sig)
{
    pid_t pid;
    int stat;
    while((pid = waitpid(-1, &stat, WNOHANG)) > 0)
        printf("child %d terminated\n", pid);
    return ;        
}

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("usage: %s ip_address port_number\n", basename(argv[0]));
        return -1;
    }

    const char* ip = argv[1];
    int port = atoi(argv[2]);
    assert(port > 0);

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    //servaddr.sin_addr.s_addr = inet_addr(ip); 
    inet_pton(AF_INET, ip, &servaddr.sin_addr);
    servaddr.sin_port = htons(port);

    int sockserv = socket(AF_INET, SOCK_STREAM, 0);
    if(sockserv < 0)
    {
        printf("socket error!\n");
        return -1;
    }

    if(bind(sockserv, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
    {
        printf("bind error!\n");
        return -1;
    }

    if(listen(sockserv, 1000) < 0)
    {
        printf("listen error!\n");
        return -1;
    }

    signal(SIGCHLD, sig_chld);

    socklen_t clilen = sizeof(servaddr);
    int sockcli;
    int num = 0;
    while(1)
    {
        if((sockcli = accept(sockserv, (struct sockaddr*)&servaddr, &clilen)) < 0)
        {
            printf("accept error!\n");
            return -1;
        }

        printf("connection from %d\n", sockcli);

        pid_t pid;
        pid = fork();
        if(pid < 0)
        {
            printf("fork error!\n");
            return -1;
        }
        else if(pid == 0)//child
        {
            const char* hello = "hello from serv\n";
            write(sockcli, hello, strlen(hello));
            close(sockcli);
            exit(0);
        }
        else//parent
        {
            close(sockcli);
        }
    }

    return 0;
}