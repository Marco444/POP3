#include "../pop3_states.h"
#include "../new_connection/pop3.h"
#include <stdio.h>

void on_arrival_force_quit(const unsigned state, struct selector_key *key){ 
    selector_unregister_fd(key->s, key->fd);
    close(key->fd);
    clean_user_data(key->data);
}
void on_departure_force_quit(const unsigned state, struct selector_key *key){ return; }
enum pop3_states on_read_ready_force_quit(struct selector_key *key){
     return AUTHORIZATION_STATE;
}
enum pop3_states on_write_ready_force_quit(struct selector_key *key){ return 0; }
enum pop3_states on_block_ready_force_quit(struct selector_key *key){ return 0; }