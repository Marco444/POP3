#include "../lib/selector/selector.h"
#include "./pop3_states.h"
#include "pop3.h"
#include "parser.h"
#include "shared.h"
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

void pop3_read(struct selector_key * key);
void pop3_write(struct selector_key * key);
void pop3_close(struct selector_key * key);
void pop3_block(struct selector_key * key);


void pop3_read(struct selector_key * key) {

}
void pop3_write(struct selector_key * key) {

}
void pop3_close(struct selector_key * key) {

}
void pop3_block(struct selector_key * key) {

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
    clientData->stm.initial = AUTHORIZATION_STATE;
    clientData->stm.states = server_states;
    clientData->stm.max_state = SERVER_STATE_COUNT;
    stm_init(&clientData->stm);

    int status = selector_register(key->s, newClientSocket, pop3State(), OP_READ, clientData);

    if (status != SELECTOR_SUCCESS) {
        close(newClientSocket);
        free(clientData);
        return;
    }

}
