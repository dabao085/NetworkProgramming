/*
 * 
*/

#ifndef _REACTOR_NETWORK_H_
#define _REACTOR_NETWORK_H_

#include <sys/socket.h>    
#include <netinet/in.h>    
#include <arpa/inet.h>    
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <sys/epoll.h>

int Connect(const char *ip_address, int port, int *fd);

int setnonblocking(int fd);

#endif