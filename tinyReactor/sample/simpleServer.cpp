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

const int MAXBUFF = 1024;

char g_buff[MAXBUFF];
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

    //set sockserv as nonblocking
    setnonblocking(sockserv);
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
    bzero(g_buff, sizeof(g_buff));
    int ret = read(fd, g_buff, sizeof(g_buff));
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
        // writeCallback(loop, fd, NULL);
    }
}

void writeCallback(CEventLoop* loop, int fd, void* args)
{
    int ret = write(fd, g_buff, strlen(g_buff));
    bzero(g_buff, sizeof(g_buff));
    if(ret < 0)
    {
        printf("write error!\n");
    }
    else if(ret == 0)
    {
        printf("server write 0 char...\n");
        loop->delIoev(fd, EPOLLOUT);
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