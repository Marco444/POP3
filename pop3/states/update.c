#include "../pop3_states.h"

// State function declarations for UPDATE_STATE
void on_arrival_update(const unsigned state, struct selector_key *key){ return; }
void on_departure_update(const unsigned state, struct selector_key *key){ return; }

enum pop3_states on_read_ready_update(struct selector_key *key){ 
  return read_commands(key, UPDATE_STATE); 
}

enum pop3_states on_write_ready_update(struct selector_key *key){ return 0; }
enum pop3_states on_block_ready_update(struct selector_key *key){ return 0; }

