#include "./command_service.h"
#include <stdio.h>
#include <strings.h>
monitor_command monitor_authorization_commands[] = {
    {"USERNAME", handle_monitor_username },
    {"PASSWORD", handle_monitor_password }
};

monitor_command monitor_transaction_commands[] = {
    {"METRICS", handle_monitor_metrics },
    {"EXIT", handle_monitor_exit },
    {"ADD_USER", handle_monitor_add_user }
};

pop3_command transaction_commands[] = {
    { "STAT", handle_stat },
    { "LIST", handle_list },
    { "RETR", handle_retr },
    { "DELE", handle_dele },
    { "NOOP", handle_noop },
    { "RSET", handle_rset },
    { "CAPA", handle_capa},
    { "QUIT", handle_transaction_quit},
};

pop3_command authorization_commands[] = {
    {"USER", handle_user},
    {"PASS", handle_pass},
    {"CAPA", handle_authorization_capa},

    {"QUIT", handle_authorization_quit}
};


pop3_command update_commands[] = {
    {"QUIT", handle_update_quit}
};

static enum pop3_states process_command_acc(pop3_command commands[], size_t num_commands, struct commands_state * ctx,struct selector_key *key) {
    size_t i = 0;
    for (i = 0; i < num_commands; i++) {
        if (strcasecmp(ctx->cmd, commands[i].name) == 0) {
            printf("Command %s\n", commands[i].name);
            return commands[i].handler(ctx,key);
        }
    }

    return ERROR_STATE; 
}

static enum monitor_states process_command_acc_monitor(monitor_command commands[], size_t num_commands, struct commands_state * ctx,struct selector_key *key) {
    size_t i = 0;
    for (i = 0; i < num_commands; i++) {
        if (strcasecmp(ctx->cmd, commands[i].name) == 0) {
            printf("Command %s\n", commands[i].name);
            return commands[i].handler(ctx,key);
        }
    }

    return ERROR_MONITOR; 
}

enum pop3_states process_command(struct commands_state *ctx,struct selector_key *key, enum pop3_states pop3_state) {
    switch(pop3_state) {
        case AUTHORIZATION_STATE:
            return process_command_acc(authorization_commands,sizeof(authorization_commands) / sizeof(pop3_command) , ctx,key);
        case TRANSACTION_STATE:
            return process_command_acc(transaction_commands, sizeof(transaction_commands) / sizeof(pop3_command) , ctx,key);
        case UPDATE_STATE:
            return process_command_acc(update_commands, sizeof(update_commands) / sizeof(pop3_command) , ctx,key);
        default:
            return ERROR_STATE;
    }
}

enum monitor_states process_command_monitor(struct commands_state *ctx,struct selector_key *key, enum monitor_states pop3_state) {
    switch(pop3_state) {
        case AUTH_MONITOR:
            return process_command_acc_monitor(monitor_authorization_commands,sizeof(monitor_authorization_commands) / sizeof(pop3_command) , ctx,key);
        case TRANSACTION_MONITOR:
             return process_command_acc_monitor(monitor_transaction_commands, sizeof(monitor_transaction_commands) / sizeof(pop3_command) , ctx,key);
        default:
            return ERROR_MONITOR;
    }
}
