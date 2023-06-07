#include "../command_service.h"
#include <stdio.h>
enum pop3_states handle_user(struct commands_state * ctx) {
    elem_type elem = calloc(1,sizeof(struct cmd));
    elem->cmd_id = USER;
    elem->offset = 0;
    elem->is_done = false;
    ctx->write_data = elem;
    return AUTHORIZATION_STATE; 
}
