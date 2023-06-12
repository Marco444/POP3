
#include "../command_service.h"
#include <stdio.h>

enum pop3_states handle_dele(struct commands_state * ctx, struct selector_key *key) {
    ctx->pop3_current_command->cmd_id = DELE;
    ctx->pop3_current_command->is_finished = false;
    ctx->pop3_current_command->has_error = false;
    ctx->pop3_current_command->noop_state = true;
    return UPDATE_STATE;
    }

enum pop3_states handle_write_dele(struct selector_key *key, pop3_current_command *current_command, struct commands_state *commands) {
    return UPDATE_STATE;
}
