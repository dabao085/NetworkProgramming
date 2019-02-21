/*
 * Author: Chengxiang
 * Date: 2019-2-20
 * 服务器循环接收连接请求，新的请求用新线程处理
*/

#include "network.h"
#include <pthread.h>

const int MAX_THREAD = 20;

void *handleConnection(void *args)
{
    int sockcli = *(int*)args;
    assert(sockcli > 0);
    const char * hello = "hello from serv";
    write(sockcli, hello, strlen(hello));

    close(sockcli);
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
    int sockserv;
    if(Connect(ip, port, &sockserv) < 0)
    {
        printf("Connect error!\n");
        return -1;
    }

    struct sockaddr_in cliaddr;
    socklen_t clilen = sizeof(cliaddr);
    pthread_t threads[MAX_THREAD];
    int index = 0, ret = 0;
    int sockcli;
    while(1)
    {
        sockcli = accept(sockserv, (struct sockaddr*)&cliaddr, &clilen);
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
    
    close(sockserv);
    return 0;
}