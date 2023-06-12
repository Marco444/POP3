#include "../command_service.h"
#include "../../../lib/selector/selector.h"
#include <stdio.h>
enum pop3_states handle_list(struct commands_state * ctx, struct selector_key *key) {
    ctx->pop3_current_command->cmd_id = LIST;
    ctx->pop3_current_command->is_finished = false;
    ctx->pop3_current_command->has_error = false;
    ctx->pop3_current_command->list_state.title_sent = false;
    ctx->pop3_current_command->list_state.current_index = 0;
    ctx->pop3_current_command->list_state.argument = -1;
    if(strlen(ctx->arg1) != 0)
    {
        if (atoi(ctx->arg1) - 1 >= ctx->inbox_data.email_files_length && atoi(ctx->arg1) <= 0){
            ctx->pop3_current_command->has_error = true;
            return TRANSACTION_STATE;
        }
        ctx->pop3_current_command->list_state.argument = atoi(ctx->arg1) - 1;
    }

    return TRANSACTION_STATE;
}
