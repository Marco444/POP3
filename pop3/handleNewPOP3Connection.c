#include "./lib/selector/selector.h"
#include "pop3_states.h"
#include <sys/socket.h>
#include <unistd.h>

int handleNewPOP3Connection() {

     // Initialize the state machine
    struct state_machine stm = {
        .initial   = AUTHORIZATION_STATE,
        .states    = server_states,
        .max_state = SERVER_STATE_COUNT,
    };
    stm_init(&stm);
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

    // Consider using a function to initialize the TClientData structure.
    struct connection_state * clientData = calloc(1, sizeof(struct connection_state));
    if (clientData == NULL) {
        close(newClientSocket);
        return;
    }

    clientData->read_buffer = buffer_new(BUFFER_SIZE);
    clientData->write_buffer = buffer_new(BUFFER_SIZE);
    // clientData->parser = parser_init();

    clientData->stm.initial = AUTHORIZATION_STATE;
    clientData->stm.states = server_states;
    clientData->stm.max_state = SERVER_STATE_COUNT;
    stm_init(&clientData->stm);

    int status = selector_register(key->s, newClientSocket, getStateHandler(), OP_READ, clientData);

    if (status != SELECTOR_SUCCESS) {
        close(newClientSocket);
        free(clientData);
        return;
    }

}