#include "./command_service.h"



pop3_command commands[] = {
    { "STAT", handle_stat },
    { "LIST", handle_list },
};


void process_command(struct connection_state * ctx, char *cmd, char *arg1, char *arg2) {
    size_t num_commands = sizeof(commands) / sizeof(pop3_command);

    size_t i = 0;
    for (i = 0; i < num_commands; i++) {
        if (strcmp(cmd, commands[i].name) == 0) {
            commands[i].handler(ctx, arg1, arg2);
            return;
        }
    }

    //ERROR
}
