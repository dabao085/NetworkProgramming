/*
 * Author: Chengx
 * Date: 2019-03-12
 * Description:
*/

#ifndef _EVENT_BASE_H_
#define _EVENT_BASE_H_
#include <stdlib.h>
class CEventLoop;
typedef void io_callback(CEventLoop* loop, int fd, void* args); //io event callback function

struct CIOEvent
{
    CIOEvent():read_cb(NULL), write_cb(NULL), rcb_args(NULL), wcb_args(NULL){}
    int event;               //event fd eg. EPOLLET, EPOLLLT, EPOLLIN, EPOLLOUT
    io_callback* read_cb;   //read callback function
    io_callback* write_cb;  //write callback function
    void* rcb_args;         //extra read callback args
    void* wcb_args;         //extra write callback args
};

#endif