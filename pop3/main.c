// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../lib/metrics/metrics.h"
#include "../lib/stm/stm.h"
#include "../lib/args/args.h"
#include "../lib/selector/selector.h"
#include "../lib/sockets/sockets.h"
#include "../lib/logger/logger.h"

#include "new_connection/pop3.h"
#include "new_connection/monitor.h"
#include "monitor_states.h"

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define MAX_CONCURRENT_CONNECTIONS 1024 


static bool terminationRequested = false;
void sigterm_handler(const int signal) {
    terminationRequested = true;
}


struct sockaddr_storage pop3_server_addr;
fd_handler pop3_server_handler = {
    .handle_read = handleNewPOP3Connection,
    .handle_write = NULL,
    .handle_block = NULL,
    .handle_close = NULL
};


struct sockaddr_storage pop3_monitor_addr;
fd_handler pop3_monitor_handler = {
    .handle_read = handleNewMonitorConnection,
    .handle_write = NULL,
    .handle_block = NULL,
    .handle_close = NULL
};



// Initialize selector
struct selector_init init_data = {
    .signal = SIGALRM, 
    .select_timeout = { .tv_sec = 100, .tv_nsec = 0 }
};


int main(int argc, char** argv) {

    //seteo 
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);

    //registros las seniales para que termine con el server
    signal(SIGTERM, sigterm_handler);
    signal(SIGINT, sigterm_handler);

    // no tenemos nada que leer de stdin
    close(STDIN_FILENO);

    // Parse command line arguments
    struct pop3args args;
    parse_args(argc, argv, &args);


    //Initialize the selector
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

    //Initialize logger
    log_info("Starting the server");

    // Initialize the server socket to receive new pop3 connections and add it to the selector
    int server_socket = setupServerSocket(args, &pop3_server_addr);
    if (server_socket < 0) {
        fprintf(stderr, "Failed to initialize server socket\n");
        return 1;
    }


    ss = selector_register(selector, server_socket, &pop3_server_handler, OP_READ, &args);
    if (ss != SELECTOR_SUCCESS) {
        fprintf(stderr, "Failed to register pop3 server socket to selector: %s\n", selector_error(ss));
        return 1;
    }

    log_info("Registered the pop3 server socket to attend new connection");

    // Initialize the server socket to receive new pop3 connections and add it to the selector
    int monitor_socket = setupMonitorSocket(args, &pop3_monitor_addr);
    if (monitor_socket < 0) {
        fprintf(stderr, "Failed to initialize monitor socket\n");
        return 1;
    }

    ss = selector_register(selector, monitor_socket, &pop3_monitor_handler, OP_READ, &args);
    if (ss != SELECTOR_SUCCESS) {
        fprintf(stderr, "Failed to register pop3 monitor socket to selector: %s\n", selector_error(ss));
        return 1;
    }

    log_info("Registered the monitor server socket to attend new connection");
    metricsInit();

    // Main server loop
    while (!terminationRequested) {
        ss = selector_select(selector);
        if (ss != SELECTOR_SUCCESS) {
            fprintf(stderr, "Failed to select: %s\n", selector_error(ss));
            break;
        }
    }


    // Clean up
    selector_destroy(selector);
    selector_close();
    // close(server_socket);

    return 0;
}

