// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../../pop3_states.h"
#include "../write_buffer_helpers.h"
#include "../../../lib/metrics/metrics.h"

#include <stdio.h>
#include <stdlib.h>


void on_arrival_update(const unsigned state, struct selector_key *key) {
    struct commands_state * commands = &((struct connection_state *)key->data)->commands;
    commands->pop3_current_command->cmd_id = QUIT_UPDATE;
    commands->pop3_current_command->is_finished = false;
    commands->pop3_current_command->quit_update_state.has_error = false;
    commands->pop3_current_command->quit_update_state.has_deleted = false;
    for(int i = 0; i < commands->inbox_data.email_files_length; i++) {
        if(commands->inbox_data.email_files[i].is_deleted) {
            int result = remove(commands->inbox_data.email_files[i].path);
            metrics_register_mails_deleted();
            commands->pop3_current_command->quit_update_state.has_deleted  = true;
            if(result < 0 ) {
                commands->pop3_current_command->quit_update_state.has_error = true;
                printf("Failed to delete email file %s\n", commands->inbox_data.email_files[i].name);
            }
        }
    }

}

void on_departure_update(const unsigned state, struct selector_key *key){
    ((struct connection_state *)key->data)->commands.last_state = UPDATE_STATE;
    return; }
enum pop3_states on_read_ready_update(struct selector_key *key){ return UPDATE_STATE; }
enum pop3_states on_write_ready_update(struct selector_key *key){
    return write_command(key);
}
enum pop3_states on_block_ready_update(struct selector_key *key){ return 0; }

