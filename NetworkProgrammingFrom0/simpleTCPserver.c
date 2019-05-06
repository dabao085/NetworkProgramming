/*
 * Author: chengx
 * Date: 2019年5月6日16:32:33
 * Descrption: A simple TCP server writen in C language. When the server accept
 * a new connection, it will close it immediately.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
    int listenfd, connfd;
    socklen_t len;
    struct sockaddr_in servaddr, cliaddr;
    char buff[1024];

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if(listenfd <= 0)
    {
        perror("socket error!\n");
        return -1;
    }

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(6610);

    int ret = bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    if(ret != 0)
    {
        perror("bind error!\n");
        return -1;
    }

    ret = listen(listenfd, 1);//max listen queue size 1
    if(ret != 0)
    {
        perror("listen error\n");
        return -1;
    }

    for(;;)
    {
        len = sizeof(cliaddr);
        connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &len);
        if(connfd <= 0)
        {
            perror("accept error!\n");
            return -1;
        }

        printf("connection from %s, port %d\n",
                inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)),
                ntohs(cliaddr.sin_port)
                );
        //close client fd immediately
        close(connfd);
    }

    return 0;
}