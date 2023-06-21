#include "../../../../lib/logger/logger.h"
#include "../../command_service.h"
#include "../../../states/write_buffer_helpers.h"
#include <stdio.h>

enum monitor_states handle_monitor_list_users(struct commands_state * ctx,struct selector_key *key) {
    log_debug("LIST_USERS read");
    ctx->pop3_current_command->cmd_id = LIST_USERS;
    ctx->pop3_current_command->is_finished = false;
    ctx->pop3_current_command->has_error = false;
    ctx->pop3_current_command->noop_state = true;
    ctx->pop3_current_command->list_state.current_index = 0;
    return TRANSACTION_MONITOR;
}
enum monitor_states handle_write_list_users_monitor(struct selector_key *key, pop3_current_command *current_command, struct commands_state *commands) {
    char message[MAX_USERS_NAME_LENGHT + 2];

    struct monitor_connection_state * state = (struct monitor_connection_state *) key->data;
    pop3args* args = state->args;
    strcpy(message, "+OK\n");
    write_in_buffer_monitor(key, message, strlen(message), 0);

    for(int i = current_command->list_state.current_index; i < args->users_count; i++) {
        user current_user = args->users[i];

        snprintf(message, sizeof(message), "%s\n", current_user.name);
        if(!enters_the_buffer_monitor(key, message)){
            current_command->list_state.current_index = i;
            if(write_in_fd_monitor(key))
                return TRANSACTION_MONITOR;
        }
         
        write_in_buffer_monitor(key, message, strlen(message), 0);
    }
    strcpy(message, "\r\n");
    write_in_buffer_monitor(key, message, strlen(message), 0);
    current_command->is_finished = true;
    if(write_in_fd_monitor(key)) {
        return TRANSACTION_MONITOR;
    }

    return ERROR_MONITOR;
}

