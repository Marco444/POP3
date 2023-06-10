
#include "../commands/command_service.h"
#include "../pop3_states.h"
#include <stdio.h>
#include <sys/socket.h>
enum pop3_states read_commands(struct selector_key *key, enum pop3_states pop3_state, bool toRead) { 
    struct connection_state *conn = (struct connection_state*) key->data;
    printf("read_commands\n");
    size_t received = 0;

    if(toRead) {
        // get write pointer and available size
        size_t nbyte;
        uint8_t *write_ptr = buffer_write_ptr(&conn->commands.read_buffer, &nbyte);

        // Read from the socket into the buffer
        received = recv(key->fd, write_ptr, nbyte, 0);
    }

    enum pop3_states next_state = pop3_state;

    bool read_command = false;

    if (!toRead || received > 0) {
       
        // Update the write pointer in the buffer if read from fd
        if(toRead)
            buffer_write_adv(&conn->commands.read_buffer, received);


        while(buffer_can_read(&conn->commands.read_buffer)) {
            // get read pointer
            size_t nbyte, i;
            uint8_t *read_ptr = buffer_read_ptr(&conn->commands.read_buffer, &nbyte);
            

            // feed the parser, the parser in itself will define the transitions!
            for(i = 0; i < nbyte; i++) {
                const struct parser_event * ret = parser_feed(conn->parser, read_ptr[i], &conn->commands);

                //this means I read a valid command into the cmd[], arg1[], arg2[] arrays in conn->commands
                if(ret->type == IS_COMMAND) {
                    buffer_read_adv(&conn->commands.read_buffer, 1);
                    enum pop3_states state = process_command(&conn->commands,key,pop3_state);
                    conn->commands.arg1[0] = '\0';
                    conn->commands.arg2[0] = '\0';
                    selector_set_interest_key(key, OP_WRITE);
                    return state;
                }

                buffer_read_adv(&conn->commands.read_buffer, 1);

            }

        }

        // compact the buffer
        buffer_compact(&conn->commands.read_buffer);

    } else if (received == 0) {
      // Client closed connection
      return  FORCED_QUIT_STATE;
    } else {
      // Handle error
      return ERROR_STATE;
    }


    return next_state;
}

