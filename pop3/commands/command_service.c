#include "./command_service.h"
#include <stdio.h>

pop3_command transaction_commands[] = {
    { "STAT", handle_stat },
    { "LIST", handle_list },
    { "RETR", handle_retr },
    { "DELE", handle_dele },
    { "NOOP", handle_noop },
    { "RSET", handle_rset },
    { "QUIT", handle_transaction_quit},
};

pop3_command authorization_commands[] = {
    {"USER", handle_user},
    {"PASS", handle_pass},
    {"QUIT", handle_authorization_quit}
};


pop3_command update_commands[] = {
    {"QUIT", handle_update_quit}
};

void process_command(struct commands_state * ctx, enum pop3_states pop3_state) {
    size_t num_commands = 3; //sizeof(authorization_commands) / sizeof(pop3_command);

    size_t i = 0;
    for (i = 0; i < num_commands; i++) {
        if (strcmp(ctx->cmd, authorization_commands[i].name) == 0) {
            authorization_commands[i].handler(ctx);
            return;
        }
    }


    //ERROR
}
