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

static void process_command_acc(pop3_command commands[], size_t num_commands, struct commands_state * ctx) {
    size_t i = 0;
    for (i = 0; i < num_commands; i++) {
        if (strcmp(ctx->cmd, commands[i].name) == 0) {
            commands[i].handler(ctx);
            return;
        }
    }

    //ERROR
}

void process_command(struct commands_state *ctx, enum pop3_states pop3_state) {
    switch(pop3_state) {
        case AUTHORIZATION_STATE:
            process_command_acc(authorization_commands, sizeof(authorization_commands) / sizeof(pop3_command) , ctx);
            break;
        case TRANSACTION_STATE:
            process_command_acc(transaction_commands, sizeof(transaction_commands) / sizeof(pop3_command) , ctx);
            break;
        case UPDATE_STATE:
            process_command_acc(update_commands, sizeof(update_commands) / sizeof(pop3_command) , ctx);
            break;
        default:
            break;
    }
}
