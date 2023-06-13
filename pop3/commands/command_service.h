#ifndef COMMAND_SERVICE_HEADER
#define COMMAND_SERVICE_HEADER 

#include <stdlib.h>
#include <string.h>
#include "../pop3_states.h"

typedef struct {
    const char *name;
    enum pop3_states (*handler)(struct commands_state * ctx,struct selector_key *key);
} pop3_command;

typedef struct {
    const char *name;
    enum monitor_states (*handler)(struct commands_state * ctx,struct selector_key *key);
} monitor_command;

extern pop3_command commands[];
enum pop3_states process_command(struct commands_state * ctx,struct selector_key *key,enum pop3_states pop3_state);

enum monitor_states process_command_monitor(struct commands_state *ctx,struct selector_key *key, enum monitor_states pop3_state);

/* Lista de los comandos soportados por el servidor pop3 en el AUTHORIZATION state como definidos en RFC 1939*/
enum pop3_states handle_user(struct commands_state * ctx,struct selector_key *key);
enum pop3_states handle_write_user(struct selector_key *key, pop3_current_command *current_command, struct commands_state *commands);

enum pop3_states handle_pass(struct commands_state * ctx,struct selector_key *key);
enum pop3_states handle_write_pass(struct selector_key *key, pop3_current_command *current_command, struct commands_state *commands);

enum pop3_states handle_authorization_quit(struct commands_state * ctx,struct selector_key *key);
enum pop3_states handle_write_authorization_quit(struct selector_key *key, pop3_current_command *current_command, struct commands_state *commands);

/* Lista de los comandos soportados por el servidor pop3 en el TRANSACTION state como definidos en RFC 1939*/
enum pop3_states handle_stat(struct commands_state * ctx,struct selector_key *key);
enum pop3_states handle_write_stat(struct selector_key *key, pop3_current_command *current_command, struct commands_state *commands);

enum pop3_states handle_capa(struct commands_state * ctx,struct selector_key *key);
enum pop3_states handle_write_capa(struct selector_key *key, pop3_current_command *current_command, struct commands_state *commands);

enum pop3_states handle_list(struct commands_state * ctx,struct selector_key *key);
enum pop3_states handle_write_list(struct selector_key *key, pop3_current_command *current_command, struct commands_state *commands);

enum pop3_states handle_retr(struct commands_state * ctx,struct selector_key *key);
enum pop3_states handle_write_retr(struct selector_key *key, pop3_current_command *current_command, struct commands_state *commands);

enum pop3_states handle_dele(struct commands_state * ctx,struct selector_key *key);
enum pop3_states handle_write_dele(struct selector_key *key, pop3_current_command *current_command, struct commands_state *commands);

enum pop3_states handle_noop(struct commands_state * ctx,struct selector_key *key);
enum pop3_states handle_write_noop(struct selector_key *key, pop3_current_command *current_command, struct commands_state *commands);

enum pop3_states handle_rset(struct commands_state * ctx,struct selector_key *key);
enum pop3_states handle_write_rset(struct selector_key *key, pop3_current_command *current_command, struct commands_state *commands);

enum pop3_states handle_transaction_quit(struct commands_state * ctx,struct selector_key *key);
enum pop3_states handle_write_transaction_quit(struct selector_key *key, pop3_current_command *current_command, struct commands_state *commands);

/* Lista de los comandos soportados por el servidor pop3 en el TRANSACTION state como definidos en RFC 1939*/
enum pop3_states handle_update_quit(struct commands_state * ctx,struct selector_key *key);
enum pop3_states handle_write_update_quit(struct selector_key *key, pop3_current_command *current_command, struct commands_state *commands);


/*
 * MONITOR
 * */

enum monitor_states handle_monitor_username(struct commands_state * ctx,struct selector_key *key);

#endif
