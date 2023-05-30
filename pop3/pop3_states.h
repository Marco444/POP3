
#ifndef POP3_STATES_HEADER 
#define POP3_STATES_HEADER 

#include "../lib/stm/stm.h"
#include "../lib/buffer/buffer.h"
#include "../lib/parser/parser.h"
#include "../lib/selector/selector.h"

#define POP3_MAX_CMD_LENGTH 512 
#define POP3_MAX_ARG_LENGTH 512 
#define BUFFER_SIZE 4096

enum pop3_states {
    AUTHORIZATION_STATE = 0,
    TRANSACTION_STATE,
    UPDATE_STATE,
    ERROR_STATE,
    FORCED_QUIT_STATE,
    SERVER_STATE_COUNT, 
};

struct connection_state {
    buffer * read_buffer; 
    buffer * write_buffer; 
    
    //donde almaceno la informacion del ADT buffer 
    uint8_t in_buffer[BUFFER_SIZE+1];
    uint8_t out_buffer[BUFFER_SIZE+1];
    size_t in_buffer_length, out_buffer_length;

    //donde almaceno el comando y argumentos que rellena el parser
    //a partir de los buffers
    char cmd[POP3_MAX_CMD_LENGTH + 1];
    char arg1[POP3_MAX_ARG_LENGTH + 1];
    char arg2[POP3_MAX_ARG_LENGTH + 1];
    size_t cmd_length, arg1_length, arg2_length;

    struct parser *parser;
    struct state_machine stm;
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

extern struct state_definition server_states[];
#endif
