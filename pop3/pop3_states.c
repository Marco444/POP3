
#include "pop3_states.h"
#include <stdio.h>

struct state_definition pop3_server_states[]  = {
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

void on_arrival_auth(const unsigned state, struct selector_key *key){ 
    printf("arrive al auth");
    return; 
}

void on_departure_auth(const unsigned state, struct selector_key *key){ return; }

enum pop3_states on_read_ready_auth(struct selector_key *key) { 
    char buff[1024];
    read(key->fd, buff, 1024);
    return AUTHORIZATION_STATE; 
}

enum pop3_states on_write_ready_auth(struct selector_key *key){
    return 0; 
}
enum pop3_states on_block_ready_auth(struct selector_key *key){ return 0; }

// State function declarations for TRANSACTION_STATE
void on_arrival_trans(const unsigned state, struct selector_key *key){ return; }
void on_departure_trans(const unsigned state, struct selector_key *key){ return; }
enum pop3_states on_write_ready_trans(struct selector_key *key){ return 0; }
enum pop3_states on_block_ready_trans(struct selector_key *key){ return 0; }

// State function declarations for UPDATE_STATE
void on_arrival_update(const unsigned state, struct selector_key *key){ return; }
void on_departure_update(const unsigned state, struct selector_key *key){ return; }
enum pop3_states on_read_ready_update(struct selector_key *key){ return 0; }
enum pop3_states on_write_ready_update(struct selector_key *key){ return 0; }
enum pop3_states on_block_ready_update(struct selector_key *key){ return 0; }


void on_arrival_error(const unsigned state, struct selector_key *key){ return; }
void on_departure_error(const unsigned state, struct selector_key *key){ return; }
enum pop3_states on_read_ready_error(struct selector_key *key){ return 0; }
enum pop3_states on_write_ready_error(struct selector_key *key){ return 0; }
enum pop3_states on_block_ready_error(struct selector_key *key){ return 0; }

