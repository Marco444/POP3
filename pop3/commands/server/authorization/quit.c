// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../../command_service.h"
#include "../../../states/write_buffer_helpers.h"

#include <stdio.h>

#define QUIT_MSG_TRANSACTION "+OK server signing off\r\n"

enum pop3_states handle_authorization_quit(struct commands_state * ctx, struct selector_key *key) {
    ctx->pop3_current_command->cmd_id = QUIT_AUTH;
    ctx->pop3_current_command->is_finished = false;
    ctx->pop3_current_command->has_error = false;
    ctx->pop3_current_command->noop_state = true;
    return AUTHORIZATION_STATE;
}

enum pop3_states handle_write_authorization_quit(struct selector_key *key, pop3_current_command *current_command, struct commands_state *commands) {
    write_in_buffer(key, QUIT_MSG_TRANSACTION, strlen(QUIT_MSG_TRANSACTION), 0);
    current_command->is_finished = true;
    write_in_fd(key);
    return FORCED_QUIT_STATE;
}
