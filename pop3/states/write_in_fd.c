
#include "../commands/command_service.h"
#include "../pop3_states.h"
#include <stdio.h>
#include <sys/socket.h>
void write_in_fd(struct selector_key *key){
    int targetFd = key->fd;
    buffer* targetBuffer = &((struct connection_state *) key->data)->commands.write_buffer;


    size_t capacity;
    ssize_t sent;
    if (!buffer_can_read(targetBuffer)) {
       // return COPY;
    }
    uint8_t * readPtr = buffer_read_ptr(targetBuffer, &(capacity));
    sent = send(targetFd, readPtr, capacity, MSG_NOSIGNAL);
    if (sent <= 0) {
        // close conection
    } else {
        buffer_read_adv(targetBuffer, sent);
    }
    
    if(buffer_can_read(targetBuffer))
        selector_set_interest_key(key, OP_WRITE);
    else
        selector_set_interest_key(key, OP_READ);
    
}