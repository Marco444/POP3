#include "../command_service.h"
#include "../../states/write_buffer_helpers.h"
#include <stdio.h>

#define QUIT_MSG_TRANSACTION "+OK server signing off\r\n"

enum pop3_states handle_transaction_quit(struct commands_state * ctx, struct selector_key *key) {
    printf("QUIT\n");
    ctx->pop3_current_command->cmd_id = QUIT_TRANSACTION;
    ctx->pop3_current_command->is_finished = false;
    ctx->pop3_current_command->has_error = false;
    ctx->pop3_current_command->noop_state = true;
    return TRANSACTION_STATE;
}

enum pop3_states handle_write_transaction_quit(struct selector_key *key, pop3_current_command *current_command, struct commands_state *commands) {
    write_in_buffer(key, QUIT_MSG_TRANSACTION, strlen(QUIT_MSG_TRANSACTION), 0);
    current_command->is_finished = true;
    return UPDATE_STATE;
}
