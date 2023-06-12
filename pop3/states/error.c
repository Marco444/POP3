#include "../pop3_states.h"
#include "write_buffer_helpers.h"

#include <stdio.h>

#define ERROR_MSG "-ERR Invalid command \n"

void on_arrival_error(const unsigned state, struct selector_key *key){ 
  write_in_buffer(key, ERROR_MSG, strlen(ERROR_MSG), 0);
  return;
}
void on_departure_error(const unsigned state, struct selector_key *key){ return; }

enum pop3_states on_read_ready_error(struct selector_key *key){
     return ERROR_STATE;
}

enum pop3_states on_write_ready_error(struct selector_key *key){ return 0; }
enum pop3_states on_block_ready_error(struct selector_key *key){ return 0; }
