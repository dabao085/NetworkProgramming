/*
 * Author: Chengxiang
 * Date: 2019-2-19
 * 
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

int Connect(const char *ip_address, int port, int *fd);

#endif