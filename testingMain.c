//#include "./lib/stm/stm.h"
//#include "./lib/args/args.h"
#include "./lib/selector/selector.h"
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

int main(int argc, char** argv) {

    // Parse command line arguments
    //struct socks5args args;
    //parse_args(argc, argv, &args);

    // Initialize the server socket
    selector_status   ss      = SELECTOR_SUCCESS;
    fd_selector selector      = NULL;
    int opt = 1;
    int server_socket =  socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    if (server_socket < 0) {
        fprintf(stderr, "Failed to initialize server socket\n");
        return 1;
    }
    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_socket, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt))) {
        fprintf(stderr, "Failed to setsockopt\n");
        return 1;
    }
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_socket, (struct sockaddr*)&address, addrlen) < 0) {
        fprintf(stderr, "Unable to bind socket");
        goto finally;
    }

    if (listen(server_socket, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }




    if(selector_fd_set_nio(server_socket) == -1) {
        fprintf(stderr, "getting server socket flags");
        goto finally;
    }
    const struct selector_init conf = {
        .signal = SIGALRM,
        .select_timeout = {
            .tv_sec  = 10,
            .tv_nsec = 0,
        },
    };
    if(0 != selector_init(&conf)) {
        fprintf(stderr, "initializing selector");
       // err_msg = "initializing selector";
        goto finally;
    }

    selector = selector_new(1024);
    if(selector == NULL) {
        fprintf(stderr, "initializing selector");
        //err_msg = "unable to create selector";
        goto finally;
    }
    const struct fd_handler socksv5 = {
        .handle_read       = socksv5_passive_accept,
        .handle_write      = NULL,
        .handle_close      = NULL, // nada que liberar
    };
    ss = selector_register(selector, server_socket, &socksv5,
                                              OP_READ, NULL);


    // ESTO DE ABAJO NO VA
    int new_socket;
    if ((new_socket
         = accept(server_socket, (struct sockaddr*)&address,
                  (socklen_t*)&addrlen))
        < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    int valread;
    char buffer[1024] = { 0 };
    char message [1024] = "Cerra el orto\n";
    while (1)
    {
        valread = read(new_socket, buffer, 1024);
        printf("%s\n", buffer);
        send(new_socket, buffer, strlen(buffer), 0);
    }
    
   
  
    // closing the connected socket
    close(new_socket);
    // closing the listening socket
    shutdown(server_socket, SHUT_RDWR);
    /*
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
    */

finally:
    // Clean up

    //selector_destroy(selector);
    //selector_close();
    close(server_socket);
    return 0;
}

