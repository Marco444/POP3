#include "../pop3_states.h"
#include <stdio.h>
#include <sys/socket.h>

#include <stdio.h>
char buff[1024];

void on_arrival_auth(const unsigned state, struct selector_key *key){ 
    return; 
}

void on_departure_auth(const unsigned state, struct selector_key *key){ return; }

enum pop3_states on_read_ready_auth(struct selector_key *key) { 
    return read_commands(key, AUTHORIZATION_STATE);
}

enum pop3_states on_write_ready_auth(struct selector_key *key){
    printf("antes imprimi del buff");
    write(key->fd, buff, 1024);
    printf("imprimi del buff");
    selector_set_interest_key(key, OP_READ);
    //desactivarme para escribir si se termino el buffer
    return 0; 
}
enum pop3_states on_block_ready_auth(struct selector_key *key){ return 0; }
