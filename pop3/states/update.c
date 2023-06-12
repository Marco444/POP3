#include "../pop3_states.h"
#include "write_buffer_helpers.h"

#include <stdio.h>
#include <stdlib.h>

#define SIGNOFF_MSG "+OK POP3 server deleted the mails\r\n"

void on_arrival_update(const unsigned state, struct selector_key *key) {
    struct commands_state *commands = (struct commands_state *)key->data;

    for(int i = 0; i < commands->inbox_data.email_files_length; i++) {
        if(commands->inbox_data.email_files[i].is_deleted) {
            int result = remove(commands->inbox_data.email_files[i].path);
            if(result == 0)
                printf("Email file %s deleted successfully\n", commands->inbox_data.email_files[i].name);
            else
                fprintf(stderr, "Failed to delete email file %s\n", commands->inbox_data.email_files[i].name);
        }
    }

    bool has_place = enters_the_buffer(key, SIGNOFF_MSG);
    if (has_place) {
        int offset = write_in_buffer(key, SIGNOFF_MSG, strlen(SIGNOFF_MSG), 0);
        if (offset == -1) {
            commands->pop3_current_command->is_finished = true;
        }
    }
}

void on_departure_update(const unsigned state, struct selector_key *key){ return; }
enum pop3_states on_read_ready_update(struct selector_key *key){ return UPDATE_STATE; }
enum pop3_states on_write_ready_update(struct selector_key *key){ return 0; }
enum pop3_states on_block_ready_update(struct selector_key *key){ return 0; }

