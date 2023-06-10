#include "../command_service.h"
#include <stdio.h>
enum pop3_states handle_user(struct commands_state * ctx, struct selector_key *key) {
    ctx->pop3_current_command->cmd_id = USER;
    ctx->pop3_current_command->is_finished = false;
    ctx->pop3_current_command->has_error = false;
    ctx->pop3_current_command->noop_state = true;
    return AUTHORIZATION_STATE; 
}
