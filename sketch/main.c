#include "states.h"
#include "../stm/stm.h"
#include "../args/args.h"
#include <stdlib.h>
#include <stdio.h>

#define MAX_CONCURRENT_CONNECTIONS 1024 

static bool terminationRequested = false;

static void sigterm_handler(const int signal) {
    terminationRequested = true;
}

int main(int argc, char** argv) {

    // Parse command line arguments
    struct socks5args args;
    parse_args(argc, argv, &args);

    // Initialize the server socket
    int server_socket;// = initializeServerSocket(port);
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

    // Initialize the state machine
    struct state_machine stm = {
        .initial   = AUTHORIZATION_STATE,
        .states    = server_states,
        .max_state = SERVER_STATE_COUNT,
    };
    stm_init(&stm);

  
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
    ss = selector_register(selector, server_socket, &server_handler, OP_READ, &stm);
    if (ss != SELECTOR_SUCCESS) {
        fprintf(stderr, "Failed to register server socket to selector: %s\n", selector_error(ss));
        return 1;
    }

    // Main server loop
    while (terminationRequested) {
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

