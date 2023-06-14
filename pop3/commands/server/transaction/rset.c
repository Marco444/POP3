#include "../../command_service.h"
#include "../../../states/write_buffer_helpers.h"
#include <stdio.h>

#define RSET_MSG "+OK\r\n"
int write_in_fd(struct selector_key *key);

enum pop3_states handle_rset(struct commands_state * ctx,struct selector_key *key) {
    ctx->pop3_current_command->cmd_id = RSET;
    ctx->pop3_current_command->is_finished = false;
    ctx->pop3_current_command->has_error = false;
    ctx->pop3_current_command->noop_state = true;
    return TRANSACTION_STATE;
}
enum pop3_states handle_write_rset(struct selector_key *key, pop3_current_command *current_command, struct commands_state *commands) {

    for(int i = 0; i < commands->inbox_data.email_files_length; i++) {
       commands->inbox_data.email_files[i].is_deleted = false; 
    }

    write_in_buffer(key, RSET_MSG, strlen(RSET_MSG), 0);
    current_command->is_finished = true;
    if(write_in_fd(key)) {
        return TRANSACTION_STATE;
    }else{
        return FORCED_QUIT_STATE;
    }
}
