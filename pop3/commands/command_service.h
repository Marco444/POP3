#ifndef COMMAND_SERVICE_HEADER
#define COMMAND_SERVICE_HEADER 

#include <stdlib.h>
#include <string.h>
#include "../pop3_states.h"

typedef struct {
    const char *name;
    void (*handler)(struct connection_state *ctx, const char *arg1, const char *arg2);
} pop3_command;


void handle_stat(struct connection_state *ctx, const char *arg1, const char *arg2);

void handle_list(struct connection_state *ctx, const char *arg1, const char *arg2);


extern pop3_command commands[];

void process_command(struct connection_state * ctx, char *cmd, char *arg1, char *arg2);

#endif
