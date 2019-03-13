/*
 * Author: Chengx
 * Date: 2019-3-12
 * Description:
*/
#include "event_loop.h"

CEventLoop::CEventLoop()
{
    _epollfd = epoll_create1(0);
}

int CEventLoop::addIoev(int fd, io_callback* callback, int event, void *args)
{
    int f_oper = 0;   //EPOLL_CTL_ADD, EPOLL_CTL_MOD
    int f_ev = 0;     //EPOLLIN, EPOLLOUT, EPOLLET etc.
    evsIter it = _io_evs.find(fd);
    if(it != _io_evs.end()) //find the element which has the same file descriptor
    {
        f_oper = EPOLL_CTL_MOD;
        f_ev = it->second.event | event;
    }
    else
    {
        f_oper = EPOLL_CTL_ADD;
        f_ev = event;
    }
    if(event & EPOLLIN)
    {
        _io_evs[fd].read_cb = callback;
        _io_evs[fd].rcb_args = args;
    }
    else
    {
        _io_evs[fd].write_cb = callback;
        _io_evs[fd].wcb_args = args;
    }
    
    _io_evs[fd].event = f_ev;
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = f_ev;
    int ret = epoll_ctl(_epollfd, f_oper, fd, &ev);
    if(ret < 0)
    {
        printf("epoll_ctl error!\n");
        return -1;
    }
    return 0;
}

int CEventLoop::delIoev(int fd, int event)
{
    int f_ev = 0;
    evsIter it = _io_evs.find(fd);
    if(it == _io_evs.end()) // could not find the element
    {
        return 0;
    }
    else    //find the element!
    {
        f_ev = it->second.event & (~event);
        if(f_ev == 0)   //means the event is cleared now
        {
            _io_evs.erase(it);
            int ret = epoll_ctl(_epollfd, EPOLL_CTL_DEL, fd, NULL);
            if(ret < 0)
            {
                printf("epoll_ctl error!\n");
                return -1;
            }
        }
        else //event is not cleared
        {
            struct epoll_event ev;
            ev.data.fd = fd;
            ev.events = f_ev;
            int ret = epoll_ctl(_epollfd, EPOLL_CTL_MOD, fd, &ev);
            if(ret < 0)
            {
                printf("epoll_ctl error!\n");
                return -1;
            }
        }
    }
    return 0;
}

int CEventLoop::delIoev(int fd)
{
    evsIter it = _io_evs.find(fd);
    if(it == _io_evs.end()) //could not find the element in event list.
    {
        return 0;
    }
    else
    {
        _io_evs.erase(it);
        int ret = epoll_ctl(_epollfd, EPOLL_CTL_DEL, fd, NULL);
        if(ret != 0)
        {
            printf("epoll_ctl error!\n");
            return -1;
        }
    }
    return 0;
}

int CEventLoop::handleEvents()
{
    while(1)
    {
        evsIter iter;
        int ret = epoll_wait(_epollfd, _events, 20, -1);
        for(int i = 0; i < ret; ++i)
        {
            iter = _io_evs.find(_events[i].data.fd);
            assert(iter != _io_evs.end());
            CIOEvent *ev = &(iter->second);
            if(_events[i].events & EPOLLIN)
            {
                void *args = ev->rcb_args;
                ev->read_cb(this, _events[i].data.fd, args);
            }
            else if(_events[i].events & EPOLLOUT)
            {
                void *args = ev->wcb_args;
                ev->write_cb(this, _events[i].data.fd, args);
            }
        }
    }
    return 0;
}