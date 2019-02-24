/*
 * Author: Chengxiang
 * Date: 2019-2-22
 * chatroom服务端,使用poll处理IO复用
*/

#include "chatroom.h"
const int MAX_USERS = 1024;
const int MAX_BUFF = 1024;

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("usage: %s ip_address port_number\n", basename(argv[0]));
        return -1;
    }

    const char *ip = argv[1];
    int port = atoi(argv[2]);
    assert(port > 0);

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, ip, &servaddr.sin_addr);
    servaddr.sin_port = htons(port);

    int sockserv = socket(AF_INET, SOCK_STREAM, 0);
    assert(sockserv > 0);

    if(bind(sockserv, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
    {
        printf("bind error!\n");
        return -1;
    }

    if(listen(sockserv, 100) < 0)
    {
        printf("listen error!\n");
        return -1;
    }

    struct pollfd fds[MAX_USERS];
    for(int i = 1; i < MAX_USERS; ++i)
    {
        fds[i].fd = -1;
        fds[i].events = 0;
        fds[i].revents = 0;
    }

    fds[0].fd = sockserv;
    fds[0].events = POLLIN;
    fds[0].revents = -1;

    int connectedCount = 0;
    char buff[MAX_BUFF];
    while(1)
    {
        int ret = poll(fds, connectedCount + 1, -1);
        if(ret < 0)
        {
            printf("poll error!\n");
            break;
        }

        for(int i = 0; i < connectedCount + 1; ++i)
        {
            if((fds[i].fd == sockserv) && (fds[i].revents & POLLIN))//新连接
            {
                struct sockaddr_in cliaddr;
                socklen_t clilen = sizeof(cliaddr);
                int sockcli = accept(sockserv, (struct sockaddr*)&cliaddr, &clilen);
                assert(sockcli > 0);
                if(connectedCount >= MAX_USERS)
                {
                    const char * tooManyUserMsg = "too many users!\n";
                    ret = write(sockcli, tooManyUserMsg, strlen(tooManyUserMsg));
                    assert(ret > 0);
                    close(sockcli);
                }
                else
                {
                    connectedCount ++;
                    fds[connectedCount].fd = sockcli;
                    fds[connectedCount].events = POLLIN | POLLRDHUP | POLLERR;
                    fds[connectedCount].revents = 0;
                }                
            }
            else if(fds[i].revents & POLLIN)//已有连接
            {
                memset(buff, '\0', MAX_BUFF);
                recv(fds[i].fd, buff, MAX_BUFF, 0);

                fds[i].events = POLLOUT;
            }
            else if(fds[i].revents & POLLOUT)
            {

            }
            else if(fds[i].revents & POLLRDHUP)
            {

            }
            else if(fds[i].revents & POLLERR)
            {

            }
        }
    }

    close(sockserv);
    return 0;
}