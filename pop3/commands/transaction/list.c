#include "../command_service.h"
#include "../../../lib/selector/selector.h"
#include <stdio.h>
enum pop3_states handle_list(struct commands_state * ctx) {
    printf("LIST\n");
    elem_type elem = calloc(1,sizeof(struct cmd));
    elem->cmd_id = LIST;
    elem->offset = 0;
    elem->is_done = false;
    queue(ctx->write_list,elem);
    return TRANSACTION_STATE;
}
