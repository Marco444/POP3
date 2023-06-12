#include "../command_service.h"
#include "../../states/write_buffer_helpers.h"
#include "../../pop3_states.h"
#include <stdio.h>

#define OK_USER "+OK is a valid mailbox\r\n"
#define OK_QUIT "+OK Quit\r\n"
#define ERRORS_USER "-ERR never heard of mailbox name\r\n"

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


enum pop3_states handle_write_user(struct selector_key *key, pop3_current_command * pop3_current, struct commands_state *commands) {
    if(!pop3_current->has_error) {
        bool has_place = enters_the_buffer(key, OK_USER);
        if (has_place) {
            long offset = write_in_buffer(key, OK_USER, strlen(OK_USER), 0);
            if (offset == -1) {
                pop3_current->is_finished = true;
            }
        }
    }else{
        bool has_place = enters_the_buffer(key, ERRORS_USER);
        if (has_place) {
            long offset = write_in_buffer(key, ERRORS_USER, strlen(ERRORS_USER), 0);
            if (offset == -1) {
                pop3_current->is_finished = true;
            }
        }
    }

    return AUTHORIZATION_STATE;
}
