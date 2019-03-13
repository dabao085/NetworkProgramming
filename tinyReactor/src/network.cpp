/*
 * Author: Chengx
 * Date: 2019-3-13
 * Description:
*/

#include "network.h"
/*
 * @InputParam1 ip_address 
 * @InputParam2 port
 * @OutputParam1 sockservfd
 * 根据传入的ip_address和port完成socket的bind和listen,sockservfd返回服务端的socket描述符
*/
int Connect(const char *ip_address, int port, int *sockservfd)
{
    if((ip_address == NULL) || (port <= 0))
    {
        printf("input param error!\n");
        return -1;
    }

    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    inet_pton(AF_INET, ip_address, &servaddr.sin_addr);
    servaddr.sin_port = htons(port);

    *sockservfd = socket(AF_INET, SOCK_STREAM, 0);
    if(*sockservfd <= 0)
    {
        printf("socket error!\n");
        return -1;
    }

    if(bind(*sockservfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0)
    {
        printf("bind error!\n");
        return -1;
    }

    if(listen(*sockservfd, 100) < 0)
    {
        printf("listen error!\n");
        return -1;
    }

    return 0;
}

int setnonblocking(int fd)
{
    int oldopt = fcntl(fd, F_GETFL);
    int newopt = oldopt | O_NONBLOCK;
    fcntl(fd, F_SETFL, newopt);
    return oldopt;
}