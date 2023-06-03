#include "../command_service.h"
#include <stdio.h>
enum pop3_states handle_ehlo(struct commands_state * ctx) {
    printf("llegue EHLO\n");
    elem_type elem = calloc(1,sizeof(struct cmd));
    elem->cmd_id = EHLO;
    elem->offset = 0;
    elem->is_done = false;
    queue(ctx->write_list,elem);
    return AUTHORIZATION_STATE;
}
