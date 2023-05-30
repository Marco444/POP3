
#include "pop3_states.h"

struct state_definition server_states[]  = {
    {
        .state            = AUTHORIZATION_STATE,
        .on_arrival       = on_arrival_auth,
        .on_departure     = on_departure_auth,
        .on_read_ready    = on_read_ready_auth,
        .on_write_ready   = on_write_ready_auth,
        .on_block_ready   = on_block_ready_auth,
    },
    {
        .state            = TRANSACTION_STATE,
        .on_arrival       = on_arrival_trans,
        .on_departure     = on_departure_trans,
        .on_read_ready    = on_read_ready_trans,
        .on_write_ready   = on_write_ready_trans,
        .on_block_ready   = on_block_ready_trans,
    },
    {
        .state            = UPDATE_STATE,
        .on_arrival       = on_arrival_update,
        .on_departure     = on_departure_update,
        .on_read_ready    = on_read_ready_update,
        .on_write_ready   = on_write_ready_update,
        .on_block_ready   = on_block_ready_update,
    },
    {
        .state            = ERROR_STATE,
        .on_arrival       = on_arrival_update,
        .on_departure     = on_departure_update,
        .on_read_ready    = on_read_ready_update,
        .on_write_ready   = on_write_ready_update,
        .on_block_ready   = on_block_ready_update,
    }
};

void on_arrival_auth(const unsigned state, struct selector_key *key){ return; }
void on_departure_auth(const unsigned state, struct selector_key *key){ return; }
unsigned on_read_ready_auth(struct selector_key *key){ return 1; }
unsigned on_write_ready_auth(struct selector_key *key){ return 1; }
unsigned on_block_ready_auth(struct selector_key *key){ return 1; }

// State function declarations for TRANSACTION_STATE
void on_arrival_trans(const unsigned state, struct selector_key *key){ return; }
void on_departure_trans(const unsigned state, struct selector_key *key){ return; }
unsigned on_write_ready_trans(struct selector_key *key){ return 1; }
unsigned on_block_ready_trans(struct selector_key *key){ return 1; }

// State function declarations for UPDATE_STATE
void on_arrival_update(const unsigned state, struct selector_key *key){ return; }
void on_departure_update(const unsigned state, struct selector_key *key){ return; }
unsigned on_read_ready_update(struct selector_key *key){ return 1; }
unsigned on_write_ready_update(struct selector_key *key){ return 1; }
unsigned on_block_ready_update(struct selector_key *key){ return 1; }


void on_arrival_error(const unsigned state, struct selector_key *key){ return; }
void on_departure_error(const unsigned state, struct selector_key *key){ return; }
unsigned on_read_ready_error(struct selector_key *key){ return 1; }
unsigned on_write_ready_error(struct selector_key *key){ return 1; }
unsigned on_block_ready_error(struct selector_key *key){ return 1; }

