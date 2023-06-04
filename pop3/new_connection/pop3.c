#include "../../lib/selector/selector.h"
#include "../pop3_states.h"
#include "pop3.h"
#include "../commands/parser.h"
#include "../shared.h"
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void pop3_read(struct selector_key * key);
void pop3_write(struct selector_key * key);
void pop3_close(struct selector_key * key);
void pop3_block(struct selector_key * key);
void write_in_fd(struct selector_key *key);

void pop3_read(struct selector_key * key) {
    struct state_machine* stm = &((struct connection_state *) key->data)->stm;
    enum pop3_states st = stm_handler_read(stm, key);
    
    struct connection_state * conn =((struct connection_state *) key->data);
    while(buffer_can_read(&conn->commands.read_buffer) && buffer_can_write(&conn->commands.write_buffer)) {
        st = read_commands(key, st ,false);
    }
    //while(buffer.notEmpty()) read_commad()
    // if (st == ERROR) {
    //     closeConnection(key);
    // }
}

void pop3_write(struct selector_key * key) {
    struct state_machine* stm = &((struct connection_state *) key->data)->stm;
    const unsigned st = stm_handler_write(stm, key);
    write_in_fd(key);
    
}
void pop3_close(struct selector_key * key) {
    struct state_machine* stm = &((struct connection_state *) key->data)->stm;
    stm_handler_close(stm, key);
}
void pop3_block(struct selector_key * key) {
    struct state_machine* stm = &((struct connection_state *) key->data)->stm;
    const enum pop3_states st = stm_handler_block(stm, key);
}

static fd_handler handler = {
    .handle_read = pop3_read,
    .handle_write = pop3_write,
    .handle_close = pop3_close,
    .handle_block = pop3_block,
};


fd_handler * pop3State() {
    return &handler;
}

void handleNewPOP3Connection(struct selector_key * key) {

    struct sockaddr_storage clientAddress;
    socklen_t clientAddressLen = sizeof(clientAddress);

    int newClientSocket = accept(key->fd, (struct sockaddr*)&clientAddress, &clientAddressLen);

    if (newClientSocket < 0) {
        return;
    }

    if (newClientSocket > 1023) {
        close(newClientSocket);
        return;
    }

    struct connection_state * clientData = calloc(1, sizeof(struct connection_state));
    if (clientData == NULL) {
        close(newClientSocket);
        return;
    }


    buffer_init(&clientData->commands.read_buffer, BUFFER_SIZE, clientData->commands.in_buffer);
    buffer_init(&clientData->commands.write_buffer, BUFFER_SIZE, clientData->commands.out_buffer);
    clientData->parser = parser_init(parser_no_classes(), &pop3_parser_definition);
    clientData->commands.write_list = new_queue();
    clientData->stm.initial = AUTHORIZATION_STATE;
    clientData->stm.states = pop3_server_states;
    clientData->stm.max_state = FORCED_QUIT_STATE;
    
    stm_init(&clientData->stm);

    int status = selector_register(key->s, newClientSocket, pop3State(), OP_READ , clientData);

    if (status != SELECTOR_SUCCESS) {
        close(newClientSocket);
        free(clientData);
        return;
    }

}

void clean_user_data(void *user_data){
    struct connection_state * clientData = (struct connection_state *)user_data;
    parser_destroy(clientData->parser);
    free_queue(clientData->commands.write_list);
    free(clientData);
}
