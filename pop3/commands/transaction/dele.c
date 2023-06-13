
#include "../command_service.h"
#include "../../states/write_buffer_helpers.h"
#include <stdio.h>
#define DELE_TEXT "+OK message deleted\r\n"
#define ERRORS_DELE "-ERR no such message\r\n"
int write_in_fd(struct selector_key *key);

enum pop3_states handle_dele(struct commands_state * ctx, struct selector_key *key) {
    ctx->pop3_current_command->cmd_id = DELE;
    ctx->pop3_current_command->is_finished = false;
    ctx->pop3_current_command->has_error = false;
    if (atoi(ctx->arg1) - 1 >= ctx->inbox_data.email_files_length || atoi(ctx->arg1) <= 0) {
        ctx->pop3_current_command->has_error = true;
        return TRANSACTION_STATE;
    }
    ctx->inbox_data.email_files[atoi(ctx->arg1) - 1].is_deleted = true;
    ctx->pop3_current_command->noop_state = true;
    return TRANSACTION_STATE;
}

enum pop3_states handle_write_dele(struct selector_key *key, pop3_current_command *current_command, struct commands_state *commands) {
    if (current_command->has_error) {
        bool has_place = enters_the_buffer(key, ERRORS_DELE);
        if (has_place) {
            long offset = write_in_buffer(key, ERRORS_DELE, strlen(ERRORS_DELE), 0);
            if (offset == -1) {
                current_command->is_finished = true;
            }
        }
    }else{
        bool has_place = enters_the_buffer(key, DELE_TEXT);
        if (has_place) {
            long offset = write_in_buffer(key, DELE_TEXT, strlen(DELE_TEXT), 0);
            if (offset == -1) {
                current_command->is_finished = true;
            }

        }
    }
    if(write_in_fd(key)) {
        return TRANSACTION_STATE;
    }else{
        return FORCED_QUIT_STATE;
    }
}
