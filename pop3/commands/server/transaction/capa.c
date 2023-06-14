#include "../../command_service.h"
#include "../../../states/write_buffer_helpers.h"

#define CAPA_MESSAGE "+OK Capability list follows\r\nUSER\r\nPIPELINING\r\n.\r\n"
int write_in_fd(struct selector_key *key);

enum pop3_states handle_capa(struct commands_state * ctx, struct selector_key *key) {
    ctx->pop3_current_command->cmd_id = CAPA;
    ctx->pop3_current_command->is_finished = false;
    ctx->pop3_current_command->has_error = false;
    ctx->pop3_current_command->noop_state = true;
    return TRANSACTION_STATE;
}

enum pop3_states handle_write_capa(struct selector_key *key, pop3_current_command *current_command, struct commands_state *commands) {
    write_in_buffer(key, CAPA_MESSAGE, strlen(CAPA_MESSAGE), 0);
    current_command->is_finished = true;
    if(write_in_fd(key)) {
        return TRANSACTION_STATE;
    }else{
        return FORCED_QUIT_STATE;
    }
}
