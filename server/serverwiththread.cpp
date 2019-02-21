/*
 * Author: Chengxiang
 * Date: 2019-2-20
 * 服务器循环接收连接请求，新的请求用新线程处理
*/

#include "network.h"
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <pthread.h>

#define MAX_THREAD 20

void *handleConnection(void *args)
{
    int sockcli = *(int*)args;
    assert(sockcli > 0);
    const char * hello = "hello from serv";
    write(sockcli, hello, strlen(hello));

    pthread_exit(NULL);
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

    if(listen(sockserv, 100) < 0)
    {
        printf("listen error!\n");
        return -1;
    }

    socklen_t clilen = sizeof(servsock);
    pthread_t threads[MAX_THREAD];
    int index = 0, ret = 0;
    int sockcli;
    while(1)
    {
        sockcli = accept(sockserv, (struct sockaddr*)&servsock, &clilen);
        if(sockcli < 0)
        {
            printf("accept error!\n");
            return -1;
        }

        //handle new connection with thread
        ret = pthread_create(&threads[index], NULL, handleConnection, (void*)&sockcli);
        if(ret < 0)
        {
            printf("pthread_create error!\n");
            return -1;
        }
        printf("thread create successfully\n");
    }

    return 0;
}