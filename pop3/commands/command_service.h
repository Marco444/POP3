#ifndef COMMAND_SERVICE_HEADER
#define COMMAND_SERVICE_HEADER 

#include <stdlib.h>
#include <string.h>
#include "../pop3_states.h"

typedef struct {
    const char *name;
    void (*handler)(struct commands_state * ctx);
} pop3_command;


extern pop3_command commands[];

void process_command(struct commands_state * ctx);

void handle_stat(struct commands_state * ctx);

void handle_list(struct commands_state * ctx);

#endif
