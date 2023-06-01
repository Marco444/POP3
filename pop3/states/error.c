#include "../pop3_states.h"
#include <stdio.h>

void on_arrival_error(const unsigned state, struct selector_key *key){ return;}
void on_departure_error(const unsigned state, struct selector_key *key){ return; }
enum pop3_states on_read_ready_error(struct selector_key *key){
     return AUTHORIZATION_STATE;
}
enum pop3_states on_write_ready_error(struct selector_key *key){ return 0; }
enum pop3_states on_block_ready_error(struct selector_key *key){ return 0; }
