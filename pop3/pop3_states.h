
#ifndef POP3_STATES_HEADER 
#define POP3_STATES_HEADER 

#include "../lib/stm/stm.h"
#include "../lib/parser/parser.h"
#include "../lib/selector/selector.h"
#include "shared.h"
#include "../lib/args/args.h"
// Define the state behavior
extern struct state_definition pop3_server_states[];

struct connection_state {
    struct commands_state commands;
    struct parser * parser;
    struct state_machine stm;
    struct pop3args * args;
    struct authorization_data auth_data;
};

//common functionality to define the write/read of a command
enum pop3_states read_commands(struct selector_key *key, enum pop3_states pop3_state, bool toRead);
enum pop3_states write_command(struct selector_key *key);

// State function declarations
void on_arrival_auth(const unsigned state, struct selector_key *key);
void on_departure_auth(const unsigned state, struct selector_key *key);
enum pop3_states on_read_ready_auth(struct selector_key *key);
enum pop3_states on_write_ready_auth(struct selector_key *key);
enum pop3_states on_block_ready_auth(struct selector_key *key);

// State function declarations for TRANSACTION_STATE
void on_arrival_trans(const unsigned state, struct selector_key *key);
void on_departure_trans(const unsigned state, struct selector_key *key);
enum pop3_states on_read_ready_trans(struct selector_key *key);
enum pop3_states on_write_ready_trans(struct selector_key *key);
enum pop3_states on_block_ready_trans(struct selector_key *key);

// State function declarations for UPDATE_STATE
void on_arrival_update(const unsigned state, struct selector_key *key);
void on_departure_update(const unsigned state, struct selector_key *key);
enum pop3_states on_read_ready_update(struct selector_key *key);
enum pop3_states on_write_ready_update(struct selector_key *key);
enum pop3_states on_block_ready_update(struct selector_key *key);


void on_arrival_error(const unsigned state, struct selector_key *key);
void on_departure_error(const unsigned state, struct selector_key *key);
enum pop3_states on_read_ready_error(struct selector_key *key);
enum pop3_states on_write_ready_error(struct selector_key *key);
enum pop3_states on_block_ready_error(struct selector_key *key);


void on_arrival_force_quit(const unsigned state, struct selector_key *key);
void on_departure_force_quit(const unsigned state, struct selector_key *key);
enum pop3_states on_read_ready_force_quit(struct selector_key *key);
enum pop3_states on_write_ready_force_quit(struct selector_key *key);
enum pop3_states on_block_ready_force_quit(struct selector_key *key);
#endif
