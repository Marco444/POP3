#include "../lib/selector/selector.h"
#include "./pop3_states.h"
#include "pop3.h"
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>

// int handleNewPOP3Connection() {
//
//      // Initialize the state machine
//     struct state_machine stm = {
//         .initial   = AUTHORIZATION_STATE,
//         .states    = server_states,
//         .max_state = SERVER_STATE_COUNT,
//     };
//     stm_init(&stm);
// }

fd_handler * pop3State() {

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

    buffer_init(clientData->read_buffer, BUFFER_SIZE, clientData->in_buffer);
    buffer_init(clientData->write_buffer, BUFFER_SIZE, clientData->out_buffer);
    // clientData->parser = parser_init();

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
