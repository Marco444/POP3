
#include "../pop3_states.h"
#include <stdio.h>
#include <sys/socket.h>

enum pop3_states read_commands(struct selector_key *key, enum pop3_states pop3_state) { 
    struct connection_state *conn = (struct connection_state*) key->data;

    // get write pointer and available size
    size_t nbyte;
    uint8_t *write_ptr = buffer_write_ptr(&conn->commands.read_buffer, &nbyte);

    // Read from the socket into the buffer
    ssize_t received = recv(key->fd, write_ptr, nbyte, 0);


    if (received > 0) {
        // Update the write pointer in the buffer
        buffer_write_adv(&conn->commands.read_buffer, received);


        while(buffer_can_read(&conn->commands.read_buffer)) {
            // get read pointer
            size_t nbyte;
            uint8_t *read_ptr = buffer_read_ptr(&conn->commands.read_buffer, &nbyte);
            

            // feed the parser, the parse in itself will define the tranisitions!
            for(size_t i = 0; i < nbyte; i++) {
              parser_feed(conn->parser, read_ptr[i], &conn->commands, pop3_state);
            }

            // advance the read pointer
            buffer_read_adv(&conn->commands.read_buffer, nbyte);
        }

        // compact the buffer
        buffer_compact(&conn->commands.read_buffer);
    } else if (received == 0) {
      // Client closed connection
      // CTRL C DEL CLIENTE
      return  FORCED_QUIT_STATE;
    } else {
      // Handle error
      return ERROR_STATE;
    }

    return pop3_state;
}
