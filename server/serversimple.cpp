/*
 * Author: Chengxiang
 * Date: 2019-2-19
 * 一个简单的小型服务器程序，只有一个进程来处理连接
*/

#include "network.h"

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("usage: %s ip_addr port_number\n", basename(argv[0]));
        return -1;
    }

    const char *ip = argv[1];
    int port = atoi(argv[2]);
    int sockserv;
    if(Connect(ip, port, &sockserv) < 0)
    {
        printf("Connect error!\n");
        return -1;
    }

    struct sockaddr_in cliaddr;
    socklen_t clilen = sizeof(cliaddr);
    int sockcli = accept(sockserv, (struct sockaddr*)&cliaddr, &clilen);
    assert(sockcli > 0);

    printf("connected with %d\n", sockcli);
    const char *hello = "hello from serv";
    write(sockcli, hello, strlen(hello));

    close(sockcli);
    close(sockserv);

    return 0;
}