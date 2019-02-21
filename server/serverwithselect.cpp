/*
 * Author: Chengxiang
 * Date: 2019-2-20
 * 服务端使用select系统调用来处理接收的连接请求
*/

#include "network.h"

const int MAX_SELECT = 100;
const int MAX_BUFF = 1024;

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        printf("usage: %s ip_address port_number\n", basename(argv[0]));
        return -1;
    }

    const char * ip = argv[1];
    int port = atoi(argv[2]);
    int sockserv;
    if(Connect(ip, port, &sockserv) < 0)
    {
        printf("Connect error!\n");
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
                    struct sockaddr_in cliaddr;
                    socklen_t clilen = sizeof(cliaddr);
                    int sockcli = accept(sockserv, (struct sockaddr*)&cliaddr, &clilen);
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

    close(sockserv);
    return 0;
}