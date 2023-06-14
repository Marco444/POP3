#include "../../pop3_states.h"
#include "../write_buffer_helpers.h"
#include "../../new_connection/pop3.h"

#include <stdio.h>

#define ERROR_MSG "-ERR Invalid command \n"

void on_arrival_error(const unsigned state, struct selector_key *key){ 
  write_in_buffer(key, ERROR_MSG, strlen(ERROR_MSG), 0);
}
void on_departure_error(const unsigned state, struct selector_key *key){
    ((struct connection_state *)key->data)->commands.last_state = AUTHORIZATION_STATE;
    return;
}

enum pop3_states on_read_ready_error(struct selector_key *key){
  // selector_unregister_fd(key->s, key->fd);
  // close(key->fd);
  // clean_user_data(key->data);
  return ERROR_STATE;
}

enum pop3_states on_write_ready_error(struct selector_key *key){
    return ((struct connection_state *)key->data)->commands.last_state;
}
enum pop3_states on_block_ready_error(struct selector_key *key){ return 0; }
