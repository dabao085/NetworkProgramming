/*
 * Author: Chengxiang
 * Date: 2019-2-19
 * header file of network
*/

#ifndef _NETWORK_H_
#define _NETWORK_H_

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

void addfd(int epollfd, int fd);

void sig_handler(int sig);

void addsig(int sig);

#endif