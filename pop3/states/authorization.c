#include "../pop3_states.h"
#include "write_buffer_helpers.h"

#define OK_USER "+OK is a valid mailbox\r\n"
#define OK_PASS "+OK mailbox locked and ready\r\n"
#define OK_QUIT "+OK Quit\r\n"
#define ERRORS_USER "-ERR never heard of mailbox name\r\n"
#define ERRORS_PASS "-ERR unable to lock mailbox\r\n"

void on_arrival_auth(const unsigned state, struct selector_key *key){ 
       return; 
}

void on_departure_auth(const unsigned state, struct selector_key *key){ return; }

enum pop3_states on_read_ready_auth(struct selector_key *key) { 
    return read_commands(key, AUTHORIZATION_STATE, true);
}

enum pop3_states on_write_ready_auth(struct selector_key *key){
    pop3_current_command * pop3_current =  ((struct connection_state *)key->data)->commands.pop3_current_command;

    switch (pop3_current->cmd_id)
    {
    case USER:{
        if(!pop3_current->has_error) {
            bool has_place = enters_the_buffer(key, OK_USER);
            if (has_place) {
                long offset = write_in_buffer(key, OK_USER, strlen(OK_USER), 0);
                if (offset == -1) {
                    pop3_current->is_finished = true;
                }
            }
        }else{
            bool has_place = enters_the_buffer(key, ERRORS_USER);
            if (has_place) {
                long offset = write_in_buffer(key, ERRORS_USER, strlen(ERRORS_USER), 0);
                if (offset == -1) {
                    pop3_current->is_finished = true;
                }
            }
        }
        return AUTHORIZATION_STATE;
    }
    case PASS: {
        if (!pop3_current->has_error) {
            bool has_place = enters_the_buffer(key, OK_PASS);
            if (has_place) {
                long offset = write_in_buffer(key, OK_PASS, strlen(OK_PASS), 0);
                if (offset == -1) {
                    pop3_current->is_finished = true;
                }
            }
        } else {
            bool has_place = enters_the_buffer(key, ERRORS_PASS);
            if (has_place) {
                long offset = write_in_buffer(key, ERRORS_PASS, strlen(ERRORS_PASS), 0);
                if (offset == -1) {
                    pop3_current->is_finished = true;
                }
            }
        }
        return TRANSACTION_STATE;
    }
    default :
        break;
    }
    
    return 0; 
}

enum pop3_states on_block_ready_auth(struct selector_key *key){ 

    char buff[1] = {0};
    write(key->fd, buff, 1);
    return 0; 
}
