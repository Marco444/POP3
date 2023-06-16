// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "sockets.h"

static int initializeServerSocket(char* addr, unsigned short port, struct sockaddr_storage* res, socklen_t* socklenResult);
static int setupSocket(struct pop3args args, struct sockaddr_storage *pop3_server_addr, int server_socket, int server, socklen_t pop3_server_addrLen);

static int initializeServerSocket(char* addr, unsigned short port, struct sockaddr_storage* res, socklen_t* socklenResult) {
    int ipv6 = strchr(addr, ':') != NULL;

    if (ipv6) {
        // Parse addr as IPv6
        struct sockaddr_in6 sock6;
        memset(&sock6, 0, sizeof(sock6));

        sock6.sin6_family = AF_INET6;
        sock6.sin6_addr = in6addr_any;
        sock6.sin6_port = htons(port);
        if (inet_pton(AF_INET6, addr, &sock6.sin6_addr) != 1) {
            // log(LOG_ERROR, "Failed IP conversion for IPv6");
            return 1;
        }

        *((struct sockaddr_in6*)res) = sock6;
        *socklenResult = sizeof(struct sockaddr_in6);
        return 0;
    }

    // Parse addr as IPv4
    struct sockaddr_in sock4;
    memset(&sock4, 0, sizeof(sock4));
    sock4.sin_family = AF_INET;
    sock4.sin_addr.s_addr = INADDR_ANY;
    sock4.sin_port = htons(port);
    if (inet_pton(AF_INET, addr, &sock4.sin_addr) != 1) {
        // log(LOG_ERROR, "Failed IP conversion for IPv4");
        return 1;
    }

    *((struct sockaddr_in*)res) = sock4;
    *socklenResult = sizeof(struct sockaddr_in);
    return 0;
}

int setupServerSocket(struct pop3args args, struct sockaddr_storage* pop3_server_addr) {
    //define the address to store the socket
    memset(pop3_server_addr, 0, sizeof(*pop3_server_addr));
    socklen_t pop3_server_addrLen = sizeof(*pop3_server_addr);

    int server_socket = initializeServerSocket(args.pop3_addr, args.pop3_port, pop3_server_addr, &pop3_server_addrLen);
    int server = socket(pop3_server_addr->ss_family, SOCK_STREAM, IPPROTO_TCP);

    if(setupSocket(args, pop3_server_addr, server_socket, server, pop3_server_addrLen) < 0) 
        return -1;

    return server;
}

int setupMonitorSocket(struct pop3args args, struct sockaddr_storage* pop3_server_addr) {
    //define the address to store the socket
    memset(pop3_server_addr, 0, sizeof(*pop3_server_addr));
    socklen_t pop3_server_addrLen = sizeof(*pop3_server_addr);

    int server_socket = initializeServerSocket(args.monitor_addr, args.monitor_port, pop3_server_addr, &pop3_server_addrLen);
    int server = socket(pop3_server_addr->ss_family, SOCK_STREAM, IPPROTO_TCP);

    if(setupSocket(args, pop3_server_addr, server_socket, server, pop3_server_addrLen) < 0) 
        return -1;

    return server;
}

static int setupSocket(struct pop3args args, struct sockaddr_storage* pop3_server_addr, int server_socket, int server, socklen_t pop3_server_addrLen) {
    if (server < 0) {
        fprintf(stderr, "Unable to create socket");
        return -1;
    }

    // man 7 ip. no importa reportar nada si falla.
    // esto le dice al SO que se puede reusar inmediatamente el socket 
    setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));

    if (bind(server, (struct sockaddr*)pop3_server_addr, pop3_server_addrLen) < 0) {
        fprintf(stderr, "Unable to bind socket");
        return -1;
    }

    if (listen(server, 20) < 0) {
        fprintf(stderr, "Unable to listen");
        return -1;
    }

    // if (selector_fd_set_nio(server) == -1) {
    //     fprintf(stderr, "Getting server socket flags");
    // }
    return 0;
}
