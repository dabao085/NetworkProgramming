/*
 * Author: Chengxiang
 * Date: 2019-2-22
 * chatroom client端, 使用poll进行IO复用。client端接收标准输入和socket读端的数据，同时将标准输入读取的数据写入到socket里，将socket读取的数据写到标准输出中,输入quit退出。
*/

#include "chatroom.h"
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
    if(sockserv < 0)
    {
        printf("socket error! %s\n", strerror(errno));
        return -1;
    }

    if(connect(sockserv, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
    {
        printf("connect error!\n");
        return -1;
    }

    struct pollfd fds[2];
    fds[0].fd = 0;
    fds[0].events = POLLIN;
    fds[0].revents = 0;
    fds[1].fd = sockserv;
    fds[1].events = POLLIN | POLLRDHUP;
    fds[1].revents = 0;

    int pipefd[2];
    int ret = pipe(pipefd);
    if(ret < 0)
    {
        printf("pipe error!\n");
        return -1;
    }

    char buff[MAX_BUFF];
    while(1)
    {
        ret = poll(fds, 2, -1);
        if(ret < 0)
        {
            printf("poll error!\n");
            return -1;
        }

        if(fds[0].revents & POLLIN)//从标准输入读入
        {
            ret = splice(0, NULL, pipefd[1], NULL, MAX_BUFF, SPLICE_F_MORE | SPLICE_F_MOVE);
            assert(ret != -1);
            ret = splice(pipefd[0], NULL, sockserv, NULL, MAX_BUFF, SPLICE_F_MORE | SPLICE_F_MOVE);
            assert(ret != -1);
        }
        else if(fds[1].revents & POLLIN)//从socket里读入
        {
            memset(buff, '\0', MAX_BUFF);
            recv(fds[1].fd, buff, MAX_BUFF - 1, 0);
            printf("%s", buff);
        }
        else if(fds[1].revents & POLLRDHUP)
        {
            printf("disconnection from server\n");
            // close(fds[1].fd);
            break;
        }
    }

    close(sockserv);
    return 0;
}