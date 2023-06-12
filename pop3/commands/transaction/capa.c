#include "../command_service.h"

#define CAPA_MSG "+OK Capability list follows\nUSER\nPASS\nQUIT\nLIST\nRETR\nDELE\nNOOP\nRSET\nSTAT\nCAPA\r\n.\r\n"

enum pop3_states handle_capa(struct commands_state * ctx, struct selector_key *key) {
    ctx->pop3_current_command->cmd_id = CAPA;
    ctx->pop3_current_command->is_finished = false;
    ctx->pop3_current_command->has_error = false;
    ctx->pop3_current_command->noop_state = true;
    return TRANSACTION_STATE;
}

enum pop3_states handle_write_capa(struct selector_key *key, pop3_current_command *current_command, struct commands_state *commands) {
    return TRANSACTION_STATE;
}
