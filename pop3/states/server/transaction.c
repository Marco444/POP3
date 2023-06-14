#include "../../pop3_states.h"
#include <sys/socket.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../write_buffer_helpers.h"

enum pop3_states on_read_ready_trans(struct selector_key *key) {
    enum pop3_states state = read_commands(key, TRANSACTION_STATE, true);
    return state;
}

enum pop3_states on_write_ready_trans(struct selector_key *key) {
    return write_command(key);
}

// State function declarations for TRANSACTION_STATE
void on_arrival_trans(const unsigned state, struct selector_key *key){ return; }
void on_departure_trans(const unsigned state, struct selector_key *key){
    ((struct connection_state *)key->data)->commands.last_state = TRANSACTION_STATE;
    return;
}
enum pop3_states on_block_ready_trans(struct selector_key *key) { return 0; }

