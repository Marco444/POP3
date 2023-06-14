#ifndef SERVER_HEADER_H 
#define SERVER_HEADER_H  

#include <stdlib.h>
#include <string.h>
#include "../../pop3_states.h"

typedef struct {
    const char *name;
    enum pop3_states (*handler)(struct commands_state * ctx,struct selector_key *key);
} pop3_command;

extern pop3_command commands[];

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

#endif
