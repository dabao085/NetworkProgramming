/*
 * Author: Chengx
 * Date: 2019-3-13
 * Description:
*/
#include "event_loop.h"
#include "network.h"
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
using namespace std;

char g_buff[1024];
void readCallback(CEventLoop* loop, int fd, void* args);
void writeCallback(CEventLoop* loop, int fd, void* args);
void acceptCallback(CEventLoop* loop, int fd, void* args);

int main(int argc, char *argv[])
{
    if(argc < 3)
    {
        printf("Usage: %s ip_address port_number\n", basename(argv[0]));
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

    CEventLoop evloop;
    evloop.addIoev(sockserv, acceptCallback, EPOLLIN);
    evloop.handleEvents();

    return 0;
}

void acceptCallback(CEventLoop* loop, int fd, void* args)
{
    struct sockaddr_in addrclient;
    socklen_t clilen = sizeof(addrclient);
    int sockclient = accept(fd, (struct sockaddr*)&addrclient, &clilen);
    if(sockclient < 0)
    {
        printf("accept error!\n");
    }
    else
    {
        if(loop->addIoev(sockclient, readCallback, EPOLLIN) < 0)
        {
            printf("addIoev error!\n");
        }
    }
}

void readCallback(CEventLoop* loop, int fd, void* args)
{
    memset(g_buff, 0, 1024);
    int ret = read(fd, g_buff, 1024);
    printf("server read %d chars\n", ret);
    if(ret < 0)
    {
        printf("read error!\n");
    }
    else if(ret == 0)
    {   
        printf("disconnection from client\n");
        loop->delIoev(fd);
    }
    else
    {
        if(loop->addIoev(fd, writeCallback, EPOLLOUT) < 0)
        {
            printf("addIoev error!\n");
        }
    }
}

void writeCallback(CEventLoop* loop, int fd, void* args)
{
    int ret = write(fd, g_buff, sizeof(g_buff));
    memset(g_buff, 0, 1024);
    if(ret < 0)
    {
        printf("write error!\n");
    }
    else if(ret == 0)
    {
        //
    }
    else
    {
        printf("server write %d chars\n", ret);
        // if(loop->addIoev(fd, readCallback, EPOLLIN) < 0)
        // {
        //     printf("addIoev error!\n");
        // }
    }
}