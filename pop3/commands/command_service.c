#include "./command_service.h"



pop3_command commands[] = {
    { "STAT", handle_stat },
    { "LIST", handle_list },
};


void process_command(struct commands_state * ctx) {
    size_t num_commands = sizeof(commands) / sizeof(pop3_command);

    size_t i = 0;
    for (i = 0; i < num_commands; i++) {
        if (strcmp(ctx->cmd, commands[i].name) == 0) {
            commands[i].handler(ctx);
            return;
        }
    }

    //ERROR
}
