#include "../command_service.h"


enum pop3_states handle_rset(struct commands_state * ctx) {
    printf("RSET\n");
    elem_type elem = calloc(1,sizeof(struct cmd));
    elem->cmd_id = RSET;
    elem->offset = 0;
    elem->is_done = false;
    ctx->write_data = elem;
    return TRANSACTION_STATE;
}
