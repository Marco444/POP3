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
    enum pop3_states next_state =  read_commands(key, AUTHORIZATION_STATE, true);
    selector_set_interest_key(key, OP_WRITE);
    return TRANSACTION_STATE;
}





enum pop3_states on_write_ready_auth(struct selector_key *key){
    selector_set_interest_key(key, OP_READ);
    //desactivarme para escribir si se termino el buffer
    struct connection_state * data =(struct connection_state *) key->data;

    size_t writeLimit;      // how many bytes we want to send
    ssize_t writeCount = 0; // how many bytes where written
    uint8_t* writeBuffer;   // buffer that stores the data to be sended

    writeBuffer = buffer_read_ptr(&data->commands.write_buffer, &writeLimit);
    writeCount = send(key->fd, writeBuffer, writeLimit, MSG_NOSIGNAL);

    if (writeCount < 0) {
        return ERROR_STATE;
    }
    if (writeCount == 0) {
        return ERROR_STATE;
    }
    buffer_read_adv(&data->commands.write_buffer, writeCount);
    selector_set_interest_key(key, OP_READ);
    if (buffer_can_read(&data->commands.write_buffer)) {
        return AUTHORIZATION_STATE;
    }
    return AUTHORIZATION_STATE;
}

enum pop3_states on_block_ready_auth(struct selector_key *key){ 

    char buff[1] = {0};
    write(key->fd, buff, 1);
    return 0; 
}
