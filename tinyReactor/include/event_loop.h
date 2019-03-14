/*
 * Author: Chengx
 * Date: 2019-3-12
 * Description:
*/

#ifndef _EVENT_LOOP_H_
#define _EVENT_LOOP_H_
#include "event_base.h"
#include <sys/epoll.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unordered_map>
class CEventLoop    //support IO event only currently
{
public:
    CEventLoop();
    ~CEventLoop();
    int handleEvents();
    int addIoev(int fd, io_callback* callback, int event, void *args = NULL);
    int delIoev(int fd, int event);
    int delIoev(int fd);
private:
    int _epollfd;
    struct epoll_event _events[20];
    std::unordered_map<int, CIOEvent> _io_evs; //fd->CIOEvent
    typedef std::unordered_map<int, CIOEvent>::iterator evsIter;
};

#endif