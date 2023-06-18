#include "../../../../lib/logger/logger.h"
#include "../../command_service.h"
#include "../../../states/write_buffer_helpers.h"
#include <stdio.h>

#define MAX_USERS_MSG 60000

enum monitor_states handle_monitor_list_users(struct commands_state * ctx,struct selector_key *key) {
    log_debug("LIST_USERS read");
    ctx->pop3_current_command->cmd_id = LIST_USERS;
    ctx->pop3_current_command->is_finished = false;
    ctx->pop3_current_command->has_error = false;
    ctx->pop3_current_command->noop_state = true;
    return TRANSACTION_MONITOR;
}
enum monitor_states handle_write_list_users_monitor(struct selector_key *key, pop3_current_command *current_command, struct commands_state *commands) {
    char users[MAX_USERS_MSG];
    log_debug("LIST_USERS write");


    char message[100] = "+ OK \n";
    write_in_buffer_monitor(key, message, strlen(message), 0);
    current_command->is_finished = true;    
    if(write_in_fd_monitor(key))
        return TRANSACTION_MONITOR;
    return ERROR_MONITOR;
}
