#ifndef COMMAND_SERVICE_HEADER
#define COMMAND_SERVICE_HEADER 

#include <stdlib.h>
#include <string.h>
#include "../pop3_states.h"

typedef struct {
    const char *name;
    enum pop3_states (*handler)(struct commands_state * ctx);
} pop3_command;


extern pop3_command commands[];
enum pop3_states process_command(struct commands_state * ctx, enum pop3_states pop3_state);

/* Lista de los comandos soportados por el servidor pop3 en el AUTHORIZATION state como definidos en RFC 1939*/
enum pop3_states handle_user(struct commands_state * ctx);
enum pop3_states handle_pass(struct commands_state * ctx);
enum pop3_states handle_authorization_quit(struct commands_state * ctx);

/* Lista de los comandos soportados por el servidor pop3 en el TRANSACTION state como definidos en RFC 1939*/
enum pop3_states handle_stat(struct commands_state * ctx);
enum pop3_states handle_list(struct commands_state * ctx);
enum pop3_states handle_retr(struct commands_state * ctx);
enum pop3_states handle_dele(struct commands_state * ctx);
enum pop3_states handle_noop(struct commands_state * ctx);
enum pop3_states handle_rset(struct commands_state * ctx);
enum pop3_states handle_transaction_quit(struct commands_state * ctx);


/* Lista de los comandos soportados por el servidor pop3 en el TRANSACTION state como definidos en RFC 1939*/
enum pop3_states handle_update_quit(struct commands_state * ctx);

#endif
