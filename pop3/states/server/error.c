// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../../pop3_states.h"
#include "../write_buffer_helpers.h"
#include "../../new_connection/pop3.h"

#include <stdio.h>

#define ERROR_MSG "-ERR Invalid command \n"
int write_in_fd(struct selector_key *key);
void on_arrival_error(const unsigned state, struct selector_key *key){ 
}
void on_departure_error(const unsigned state, struct selector_key *key){
    return;
}

enum pop3_states on_read_ready_error(struct selector_key *key){
  return ERROR_STATE;
}

enum pop3_states on_write_ready_error(struct selector_key *key){
    write_in_buffer(key, ERROR_MSG, strlen(ERROR_MSG), 0);
    write_in_fd(key);
    return ((struct connection_state *)key->data)->commands.last_state;
}
enum pop3_states on_block_ready_error(struct selector_key *key){ return 0; }
