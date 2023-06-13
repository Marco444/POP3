
#ifndef SOCKETS_HEADER
#define SOCKETS_HEADER

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "./lib/args/args.h"


int initializeServerSocket(char* addr, unsigned short port, void* res, socklen_t* socklenResult);


int setupSocket(struct pop3args args , struct sockaddr_storage pop3_server_addr);

#endif
