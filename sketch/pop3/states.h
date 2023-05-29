#include "../stm/stm.h"
#include "../buffer/buffer.h"
#include "../parser/parser.h"
#include "../selector/selector.h"

#define POP3_MAX_CMD_LENGTH 512 
#define POP3_MAX_ARG_LENGTH 512 

enum pop3_states {
    AUTHORIZATION_STATE = 0,
    TRANSACTION_STATE,
    UPDATE_STATE,
    ERROR_STATE,
    SERVER_STATE_COUNT, 
};

struct connection_state {
    buffer read_buffer; 
    buffer write_buffer; 
    char cmd[POP3_MAX_CMD_LENGTH+1];
    char arg[POP3_MAX_ARG_LENGTH+1];
    size_t cmd_len;
    size_t arg_len;
    struct parser *parser;
};


// State function declarations
void on_arrival_auth(const unsigned state, struct selector_key *key);
void on_departure_auth(const unsigned state, struct selector_key *key);
unsigned on_read_ready_auth(struct selector_key *key);
unsigned on_write_ready_auth(struct selector_key *key);
unsigned on_block_ready_auth(struct selector_key *key);

// State function declarations for TRANSACTION_STATE
void on_arrival_trans(const unsigned state, struct selector_key *key);
void on_departure_trans(const unsigned state, struct selector_key *key);
unsigned on_read_ready_trans(struct selector_key *key);
unsigned on_write_ready_trans(struct selector_key *key);
unsigned on_block_ready_trans(struct selector_key *key);

// State function declarations for UPDATE_STATE
void on_arrival_update(const unsigned state, struct selector_key *key);
void on_departure_update(const unsigned state, struct selector_key *key);
unsigned on_read_ready_update(struct selector_key *key);
unsigned on_write_ready_update(struct selector_key *key);
unsigned on_block_ready_update(struct selector_key *key);


void on_arrival_error(const unsigned state, struct selector_key *key);
void on_departure_error(const unsigned state, struct selector_key *key);
unsigned on_read_ready_error(struct selector_key *key);
unsigned on_write_ready_error(struct selector_key *key);
unsigned on_block_ready_error(struct selector_key *key);

// Define the state behavior
struct state_definition server_states[] = {
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

