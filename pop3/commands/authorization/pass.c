#include "../command_service.h"
#include <stdio.h>
enum pop3_states handle_pass(struct commands_state * ctx) {
 elem_type elem = calloc(1,sizeof(struct cmd));
    elem->cmd_id = PASS;
    elem->offset = 0;
    elem->is_done = false;
    queue(ctx->write_list,elem);
    return TRANSACTION_STATE; 
}
