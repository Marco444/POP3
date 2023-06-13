#include "../../pop3_states.h"
#include "../write_buffer_helpers.h"
#define GREATING "+OK POP3 server ready\r\n"
int write_in_fd(struct selector_key *key);
void on_arrival_auth(const unsigned state, struct selector_key *key){
    write_in_buffer(key,GREATING, strlen(GREATING),0);
    return;
}

void on_departure_auth(const unsigned state, struct selector_key *key){ return; }

enum pop3_states on_read_ready_auth(struct selector_key *key) { 
    return read_commands(key, AUTHORIZATION_STATE, true);
}

enum pop3_states on_write_ready_auth(struct selector_key *key){
    if (((struct connection_state *)key->data)->commands.pop3_current_command->cmd_id == NOOP){
        write_in_fd(key);
        ((struct connection_state *)key->data)->commands.pop3_current_command->is_finished = true;
        return AUTHORIZATION_STATE;
    }
    return write_command(key);
}

enum pop3_states on_block_ready_auth(struct selector_key *key){ 
    char buff[1] = {0};
    write(key->fd, buff, 1);
    return 0; 
}
