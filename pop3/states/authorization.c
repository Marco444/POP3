#include "../pop3_states.h"
#include "write_buffer_helpers.h"


void on_arrival_auth(const unsigned state, struct selector_key *key){ 
       return;
}

void on_departure_auth(const unsigned state, struct selector_key *key){ return; }

enum pop3_states on_read_ready_auth(struct selector_key *key) { 
    return read_commands(key, AUTHORIZATION_STATE, true);
}

enum pop3_states on_write_ready_auth(struct selector_key *key){
    // return write_command(key);
    return AUTHORIZATION_STATE;
}

enum pop3_states on_block_ready_auth(struct selector_key *key){ 
    char buff[1] = {0};
    write(key->fd, buff, 1);
    return 0; 
}
