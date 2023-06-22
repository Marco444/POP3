
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

#include "../args/args.h"


int setupMonitorSocket(conection_data args , struct sockaddr_storage * pop3_server_addr);

int setupServerSocket(conection_data args, struct sockaddr_storage* pop3_server_addr) ;



#endif
