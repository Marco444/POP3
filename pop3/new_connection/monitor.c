#include "../../lib/selector/selector.h"
#include "../../lib/metrics/metrics.h"
#include "../monitor_states.h"
#include "monitor.h"
#include "../commands/parser.h"
#include "../shared.h"
#include "../../lib/stm/stm.h"
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>


void pop3_monitor_read(struct selector_key * key);
void pop3_monitor_write(struct selector_key * key);
void pop3_monitor_close(struct selector_key * key);
void pop3_monitor_block(struct selector_key * key);
void write_in_fd(struct selector_key *key);

void pop3_monitor_read(struct selector_key * key) {
    struct state_machine* stm = &((struct monitor_connection_state *) key->data)->stm;
    stm_handler_read(stm, key);
}

void pop3_monitor_write(struct selector_key * key) {
    struct state_machine* stm = &((struct monitor_connection_state *) key->data)->stm;
   stm_handler_write(stm, key);
    // write_in_fd(key);
    // pop3_monitor_current_command *current_command = ((struct connection_state *) key->data)->commands.pop3_current_command;
    // if(current_command->is_finished && !buffer_can_read(&((struct connection_state *) key->data)->commands.write_buffer)) {
    //     if(buffer_can_read(&((struct connection_state *) key->data)->commands.read_buffer)){
    //         read_commands(key, st, false);
    //         selector_set_interest(key->s, key->fd, OP_WRITE);
    //         return;
    //     }
    //     selector_set_interest(key->s, key->fd, OP_READ);
    //     return;
    // }
    // if(current_command->cmd_id == RETR){
    //     if (buffer_can_read(&((struct connection_state *) key->data)->commands.write_buffer)) {
    //         selector_set_interest(key->s, key->fd, OP_WRITE);
    //     } else {
    //         selector_set_interest(key->s, current_command->retr_state.mail_fd, OP_READ);
    //         selector_set_interest(key->s, key->fd, OP_NOOP);
    //     }
    // }
}
void pop3_monitor_close(struct selector_key * key) {
    struct state_machine* stm = &((struct monitor_connection_state *) key->data)->stm;
    stm_handler_close(stm, key);
}
void pop3_monitor_block(struct selector_key * key) {
    struct state_machine* stm = &((struct monitor_connection_state *) key->data)->stm;
    stm_handler_block(stm, key);
}

static fd_handler handler = {
    .handle_read = pop3_monitor_read,
    .handle_write = pop3_monitor_write,
    .handle_close = pop3_monitor_close,
    .handle_block = pop3_monitor_block,
};

fd_handler * pop3_monitorState(void) {
    return &handler;
}
void handleNewMonitorConnection(struct selector_key * key) {

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

   struct monitor_connection_state * clientData = calloc(1, sizeof(struct monitor_connection_state));
    if (clientData == NULL) {
        close(newClientSocket);
        return;
    }


    buffer_init(&clientData->commands.read_buffer, BUFFER_SIZE, clientData->commands.in_buffer);
    buffer_init(&clientData->commands.write_buffer, BUFFER_SIZE, clientData->commands.out_buffer);
    clientData->parser = parser_init(parser_no_classes(), &pop3_parser_definition);
    //clientData->commands.pop3_monitor_current_command = calloc(1,sizeof(struct pop3_current_command));
    clientData->stm.initial = AUTH_MONITOR;
    clientData->stm.states =  pop3_monitor_states;
    clientData->stm.max_state = QUIT_MONITOR;
    clientData->auth_data.user_index = -1;
    clientData->auth_data.is_logged = false;
    clientData->args = key->data;

    stm_init(&clientData->stm);


    int status = selector_register(key->s, newClientSocket, pop3_monitorState(), OP_READ , clientData);

    if (status != SELECTOR_SUCCESS) {
        close(newClientSocket);
        free(clientData);
        return;
    }

    metricsRegisterNewClient();

}
