#include "../command_service.h"
#include "../../../lib/selector/selector.h"
#include <stdio.h>
enum pop3_states handle_list(struct commands_state * ctx, struct selector_key *key) {
    ctx->pop3_current_command->cmd_id = LIST;
    ctx->pop3_current_command->is_finished = false;
    ctx->pop3_current_command->has_error = false;
    ctx->pop3_current_command->list_state.title_sent = false;
    ctx->pop3_current_command->list_state.current_index = 0;
    return TRANSACTION_STATE;
}
