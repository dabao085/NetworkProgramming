/*
 * Author: Chengxiang
 * Date: 2019-2-19
 * 一个简单的小型服务器程序，只有一个进程来处理连接
*/

#include "network.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("usage: %s ip_addr port_number\n", basename(argv[0]));
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

    if(listen(sockserv, 10) < 0)
    {
        printf("listen error!\n");
        return -1;
    }

    socklen_t clilen = sizeof(servaddr);
    int sockcli = accept(sockserv, (struct sockaddr*)&servaddr, &clilen);
    assert(sockcli > 0);

    printf("connected with %d\n", sockcli);
    const char *hello = "hello from serv";
    write(sockcli, hello, strlen(hello));

    close(sockcli);

    return 0;
}