#include "monitor/monitor_states.h"
#include "lib/stm/stm.h"
#include "./lib/args/args.h"
#include "./lib/selector/selector.h"
#include "./pop3/pop3.h"
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

#define MAX_CONCURRENT_CONNECTIONS 1024 

static bool terminationRequested = false;

static void sigterm_handler(const int signal) {
    terminationRequested = true;
}

static int initializeServerSocket(char* addr, unsigned short port, void* res, socklen_t* socklenResult) {
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

static int setupSocket(struct pop3args args , struct sockaddr_storage auxAddr, socklen_t auxAddrLen) {

    int server_socket = initializeServerSocket(args.pop3_addr, args.pop3_port, &auxAddr, &auxAddrLen);
    int server = socket(auxAddr.ss_family, SOCK_STREAM, IPPROTO_TCP);

    if (server < 0) {
        fprintf(stderr, "Unable to create socket");
        return -1;
    }

    // man 7 ip. no importa reportar nada si falla.
    setsockopt(server, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));

    if (bind(server, (struct sockaddr*)&auxAddr, auxAddrLen) < 0) {
        fprintf(stderr, "Unable to bind socket");
        return -1;
    }

    if (listen(server, 20) < 0) {
        fprintf(stderr, "Unable to listen");
        return -1;
    }

    // if (selector_fd_set_nio(server) == -1) {
    //     fprintf(stderr, "Getting server socket flags";
    // }
    return 0;
}


int main(int argc, char** argv) {

    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);

    // no tenemos nada que leer de stdin
    close(STDIN_FILENO);


    // Parse command line arguments
    struct pop3args args = {
        .pop3_port = 8080,
        .pop3_addr = "0.0.0.0"
    };
    //parse_args(argc, argv, &args);


    //define the address to store the socket
    struct sockaddr_storage auxAddr;
    memset(&auxAddr, 0, sizeof(auxAddr));
    socklen_t auxAddrLen = sizeof(auxAddr);
    int server = -1;

    // Initialize the server socket
    int server_socket = setupSocket(args, auxAddr, auxAddrLen);
    if (server_socket < 0) {
        fprintf(stderr, "Failed to initialize server socket\n");
        return 1;
    }

    // Initialize selector
    struct selector_init init_data = {
        .signal = SIGALRM, 
        .select_timeout = { .tv_sec = 1, .tv_nsec = 0 }
    };

    selector_status ss = selector_init(&init_data);
    if (ss != SELECTOR_SUCCESS) {
        fprintf(stderr, "Failed to initialize selector: %s\n", selector_error(ss));
        return 1;
    }

    fd_selector selector = selector_new(MAX_CONCURRENT_CONNECTIONS);
    if (selector == NULL) {
        fprintf(stderr, "Failed to create selector\n");
        return 1;
    }
  
    //Initialize logging for server


    //registros las seniales para que termine con el server
    signal(SIGTERM, sigterm_handler);
    signal(SIGINT, sigterm_handler);

    // Register the server socket to the selector
    fd_handler server_handler = {
        .handle_read = handleNewPOP3Connection,
        .handle_write = NULL,
        .handle_block = NULL,
        .handle_close = NULL
    };

    ss = selector_register(selector, server_socket, &server_handler, OP_READ, NULL);
    if (ss != SELECTOR_SUCCESS) {
        fprintf(stderr, "Failed to register server socket to selector: %s\n", selector_error(ss));
        return 1;
    }

    // Main server loop
    while (!terminationRequested) {
        ss = selector_select(selector);
        if (ss != SELECTOR_SUCCESS) {
            fprintf(stderr, "Failed to select: %s\n", selector_error(ss));
            break;
        }
    }


finally:
    // Clean up
    selector_destroy(selector);
    selector_close();
    close(server_socket);

    return 0;
}

