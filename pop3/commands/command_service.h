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

/* Lista de los comandos soportados por el servidor pop3 en el TRANSACTION state */

void process_command(struct commands_state * ctx);

void handle_stat(struct commands_state * ctx);

void handle_list(struct commands_state * ctx);

void handle_retr(struct commands_state * ctx);

void handle_dele(struct commands_state * ctx);

void handle_noop(struct commands_state * ctx);

void handle_rset(struct commands_state * ctx);

void handle_quit(struct commands_state * ctx);

#endif
