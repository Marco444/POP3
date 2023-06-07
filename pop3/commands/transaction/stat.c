#include "../command_service.h"
#include <stdio.h>
enum pop3_states handle_stat(struct commands_state * ctx) {
     printf("STAT\n");
    elem_type elem = calloc(1,sizeof(struct cmd));
    elem->cmd_id = STAT;
    elem->offset = 0;
    elem->is_done = false;
    ctx->write_data = elem;
    return TRANSACTION_STATE;
}
