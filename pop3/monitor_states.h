
#include "../lib/stm/stm.h"
#include "../lib/parser/parser.h"
#include "../lib/selector/selector.h"
#include "shared.h"
#include "../lib/args/args.h"


extern struct state_definition pop3_monitor_states[];

struct monitor_connection_state {
    struct parser * parser;
    struct state_machine stm;
    struct pop3args * args;
    struct authorization_data auth_data;
    struct commands_state commands;
};

// State function declarations
void on_arrival_auth_monitor(const unsigned state, struct selector_key *key);
void on_departure_auth_monitor(const unsigned state, struct selector_key *key);
enum monitor_states on_read_ready_auth_monitor(struct selector_key *key);
enum monitor_states on_write_ready_auth_monitor(struct selector_key *key);
enum monitor_states on_block_ready_auth_monitor(struct selector_key *key);

// State function declarations for TRANSACTION_STATE
void on_arrival_trans_monitor(const unsigned state, struct selector_key *key);
void on_departure_trans_monitor(const unsigned state, struct selector_key *key);
enum monitor_states on_read_ready_trans_monitor(struct selector_key *key);
enum monitor_states on_write_ready_trans_monitor(struct selector_key *key);
enum monitor_states on_block_ready_trans_monitor(struct selector_key *key);


void on_arrival_quit_monitor(const unsigned state, struct selector_key *key);
void on_departure_quit_monitor(const unsigned state, struct selector_key *key);
enum monitor_states on_read_ready_quit_monitor(struct selector_key *key);
enum monitor_states on_write_ready_quit_monitor(struct selector_key *key);
enum monitor_states on_block_ready_quit_monitor(struct selector_key *key);
