// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../../commands/command_service.h"
#include "../../monitor_states.h"
#include "../../commands/parser.h"
#include <stdio.h>
#include <sys/socket.h>

typedef enum monitor_states (*command_handler)(struct selector_key *key, pop3_current_command *current_command, struct commands_state *commands);

static command_handler monitor_command_handlers[MONITOR_CMD_COUNT] = {
    [USERNAME] = handle_write_username_monitor,
    [PASSWORD] = handle_write_password_monitor,
    [METRICS] = handle_write_metrics_monitor,
    [EXIT] = handle_write_exit_monitor,
    [ADD_USER] = handle_write_add_user_monitor,
};

enum monitor_states write_command_monitor(struct selector_key *key) {
    pop3_current_command *current_command = ((struct monitor_connection_state *) key->data)->commands.pop3_current_command;
    struct commands_state *commands = &((struct monitor_connection_state *) key->data)->commands;

    if ( current_command->cmd_id < MONITOR_CMD_COUNT)
        return monitor_command_handlers[current_command->cmd_id](key, current_command, commands);

    return ERROR_MONITOR;
}

enum monitor_states read_commands_monitor(struct selector_key *key, enum monitor_states pop3_state, bool toRead) { 
    struct monitor_connection_state *conn = (struct monitor_connection_state*) key->data;
    size_t received = 0;

    if(toRead) {
        // get write pointer and available size
        size_t nbyte;
        uint8_t *write_ptr = buffer_write_ptr(&conn->commands.read_buffer, &nbyte);

        // Read from the socket into the buffer
        received = recv(key->fd, write_ptr, nbyte, 0);
    }

    enum monitor_states  next_state = pop3_state;


    if (!toRead || received > 0) {
       
        // Update the write pointer in the buffer if read from fd
        if(toRead)
            buffer_write_adv(&conn->commands.read_buffer, received);


        while(buffer_can_read(&conn->commands.read_buffer)) {
            // get read pointer
            size_t nbyte, i;
            uint8_t *read_ptr = buffer_read_ptr(&conn->commands.read_buffer, &nbyte);
            

            for(i = 0; i < nbyte; i++) {
                const struct parser_event * ret = parser_feed(conn->parser, read_ptr[i], &conn->commands);

                //this means I read a valid command into the cmd[], arg1[], arg2[] arrays in conn->commands
                if(ret->type == IS_COMMAND) {
                    buffer_read_adv(&conn->commands.read_buffer, 1);
                    enum monitor_states state = process_command_monitor(&conn->commands,key,pop3_state);
                    reset_buffers(&conn->commands);
                    selector_set_interest_key(key, OP_WRITE);
                    return state;
                }

                if(ret->type == INVALID_COMMAND) {
                    buffer_read_adv(&conn->commands.read_buffer, 1);
                    reset_buffers(&conn->commands);
                    selector_set_interest_key(key, OP_WRITE);
                    return ERROR_MONITOR;
                }

                buffer_read_adv(&conn->commands.read_buffer, 1);

            }

        }

        // compact the buffer
        buffer_compact(&conn->commands.read_buffer);

    } else if (received == 0) {
      // Client closed connection
      return  ERROR_MONITOR;
    } else {
      // Handle error
      return ERROR_MONITOR;
    }


    return next_state;
}

