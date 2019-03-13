/*
 * Author: Chengx
 * Data: 2019-3-12
 * Description:
*/

#include "event_loop.h"
#include <iostream>
using namespace std;

void readCallback(CEventLoop* loop, int fd, void* args)
{
    char buff[1024];
    memset(buff, 0, 1024);
    int ret = read(fd, buff, 1024);
    assert(ret >= 0);
    cout << "callback: " << buff;
}

int main(int argc, char *argv[])
{
    CEventLoop evloop;
    evloop.addIoev(0, readCallback, EPOLLIN);
    evloop.handleEvents();

    return 0;
}