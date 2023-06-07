
#include "../command_service.h"
#include <stdio.h>

enum pop3_states handle_retr(struct commands_state * ctx) {
    printf("RETR\n");
    elem_type elem = calloc(1,sizeof(struct cmd));
    elem->cmd_id = RETR;
    elem->offset = 0;
    elem->is_done = false;
    ctx->write_data = elem;
    return TRANSACTION_STATE; 
}
