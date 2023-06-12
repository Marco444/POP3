#include "../command_service.h"
#include <stdio.h>
enum pop3_states handle_authorization_quit(struct commands_state * ctx, struct selector_key *key) {
    ctx->pop3_current_command->cmd_id = QUIT_AUTH;
    ctx->pop3_current_command->is_finished = false;
    ctx->pop3_current_command->has_error = false;
    ctx->pop3_current_command->noop_state = true;
    return AUTHORIZATION_STATE;
}

enum pop3_states handle_write_authorization_quit(struct selector_key *key, pop3_current_command *current_command, struct commands_state *commands) {
    return TRANSACTION_STATE;
}
