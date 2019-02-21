/*
 * Author: Chengxiang
 * Date: 2019-2-19
 * 循环接收新连接，每接收一个就开一个进程来处理连接
*/

#include "network.h"

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
    int sockserv;
    if(Connect(ip, port, &sockserv) < 0)
    {
        printf("Connect error!\n");
        return -1;
    }

    signal(SIGCHLD, sig_chld);

    struct sockaddr_in cliaddr;
    socklen_t clilen = sizeof(cliaddr);
    int sockcli, num = 0;
    while(1)
    {
        if((sockcli = accept(sockserv, (struct sockaddr*)&cliaddr, &clilen)) < 0)
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

    close(sockserv);
    return 0;
}