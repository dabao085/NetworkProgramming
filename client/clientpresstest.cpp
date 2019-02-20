/*
 * Author: Chengxiang
 * Date: 2019-2-19
 * 一个简单的压测工具
*/

#include "network.h"
#include <iostream>
#include <stdio.h>
#include <string>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

int connectAndSend(const char *ip, int port)
{
    struct sockaddr_in clientaddr;
    memset(&clientaddr, 0, sizeof(clientaddr));
    clientaddr.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &clientaddr.sin_addr);
    clientaddr.sin_port = htons(port);

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    assert(sockfd > 0);

    if(connect(sockfd, (struct sockaddr*)&clientaddr, sizeof(clientaddr)) < 0)
    {
        printf("connect error!\n");
        return -1;
    }

    //send
    const char *hello = "hello from client";
    send(sockfd, hello, sizeof(hello), 0);

    return 0;
}

int main(int argc, char *argv[])
{
    if(argc < 3)
    {
        printf("usage: %s ip_address port_number process_num\n", basename(argv[0]));
        return -1;
    }

    const char *ip = argv[1];
    int port = atoi(argv[2]);
    int processNum = atoi(argv[3]);

    assert(port > 0 && processNum > 0);

    //fork
    pid_t pid;
    int status;
    for(int i = 0; i < processNum; ++i)
    {
        pid = fork();
        if(pid < 0)
        {
            printf("fork error\n");
            break;
        }
        else if(pid == 0)//child
        {
            //向服务端发送请求
            if(connectAndSend(ip, port) < 0)
            {
                printf("connectAndSend error\n");
                return -1;
            }
            exit(0);//exit
        }
        else//parent
        {
            //do nothing
        }
    }

    if(pid > 0)
    {
        for(int i = 0; i < processNum; ++i)
        {
            pid_t p = wait(&status);
            printf("process %d finished\n", p);
        }
    }
    /*
        while(wait(NULL) > 0)
            ;
    */

    return 0;
}
