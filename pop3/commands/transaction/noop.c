#include "../command_service.h"
#include <stdio.h>
enum pop3_states handle_noop(struct commands_state * ctx, struct selector_key *key) {
    printf("NOOP\n");
    ctx->pop3_current_command->cmd_id = NOOP;
    ctx->pop3_current_command->is_finished = false;
    ctx->pop3_current_command->has_error = false;
    ctx->pop3_current_command->noop_state = true;
    return TRANSACTION_STATE;
}
enum pop3_states handle_write_noop(struct selector_key *key, pop3_current_command *current_command, struct commands_state *commands) {
    return TRANSACTION_STATE;
}
