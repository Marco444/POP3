#include "../pop3_states.h"
#include <sys/socket.h>


// State function declarations for TRANSACTION_STATE
void on_arrival_trans(const unsigned state, struct selector_key *key){ return; }
void on_departure_trans(const unsigned state, struct selector_key *key){ return; }
enum pop3_states on_write_ready_trans(struct selector_key *key){ return 0; }
enum pop3_states on_block_ready_trans(struct selector_key *key){ return 0; }

unsigned on_read_ready_trans(struct selector_key *key) {
    return read_commands(key, TRANSACTION_STATE, true);
}
