/*
 * Author: Chengxiang
 * Date: 2019-2-20
 * 服务端使用select系统调用来处理接收的连接请求
*/

#include "network.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

#define MAX_SELECT 100
#define MAX_BUFF 1024

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("usage: %s ip_address port_number\n", basename(argv[0]));
        return -1;
    }

    const char * ip = argv[1];
    int port = atoi(argv[2]);
    assert(port > 0);

    struct sockaddr_in servsock;
    memset(&servsock, 0, sizeof(servsock));
    servsock.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &servsock.sin_addr);
    servsock.sin_port = htons(port);

    int sockserv = socket(AF_INET, SOCK_STREAM, 0);
    if(sockserv < 0)
    {
        printf("socket error!\n");
        return -1;
    }

    if(bind(sockserv, (struct sockaddr*)&servsock, sizeof(servsock)) < 0)
    {
        printf("bind error!\n");
        return -1;
    }

    if(listen(sockserv, 1) < 0)
    {
        printf("listen error!\n");
        return -1;
    }

    fd_set active_fd_set, read_fd_set;
    FD_ZERO(&active_fd_set);
    FD_SET(sockserv, &active_fd_set);

    while(1)
    {
        read_fd_set = active_fd_set;
        if(select(MAX_SELECT, &read_fd_set, NULL, NULL, NULL) < 0)
        {
            printf("select error!\n");
            return -1;
        }

        for(int i = 0; i < MAX_SELECT; ++i)
        {
            if(FD_ISSET(i, &read_fd_set))
            {
                if(i == sockserv)
                {
                    //新连接
                    socklen_t clilen = sizeof(servsock);
                    int sockcli = accept(sockserv, (struct sockaddr*)&servsock, &clilen);
                    assert(sockcli > 0);

                    FD_SET(sockcli, &active_fd_set);
                }
                else//已有连接
                {
                    char buff[MAX_BUFF];
                    int ret = read(i, buff, MAX_BUFF);
                    if(ret > 0)
                        printf("RecvBuff: %s\n", buff);
                    else
                    {
                        printf("close connection from %d\n", i);
                        close(i);
                        FD_CLR(i, &active_fd_set);
                    }
                }
            }
        }
    }

    return 0;
}