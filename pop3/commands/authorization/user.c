#include "../command_service.h"
#include "../pop3_states.h"
#include <stdio.h>
enum pop3_states handle_user(struct commands_state * ctx, struct selector_key *key) {
    ctx->pop3_current_command->cmd_id = USER;
    ctx->pop3_current_command->is_finished = false;
    ctx->pop3_current_command->has_error = false;
    ctx->pop3_current_command->noop_state = true;
    struct connection_state * state = (struct connection_state *) key->data;
    for (int i = 0; state->args->users[i].name != NULL; ++i) {
        if (strcmp(state->args->users[i].name,ctx->arg1) == 0){
            state->auth_data.user_index = i;
            state->auth_data.is_logged = false;
            return AUTHORIZATION_STATE;
        }
    }
    state->auth_data.user_index = -1;
    state->auth_data.is_logged = false;
    ctx->pop3_current_command->has_error = true;
    return AUTHORIZATION_STATE; 
}
