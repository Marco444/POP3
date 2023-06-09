// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../../pop3_states.h"
#include "../../../lib/metrics/metrics.h"
#include "../../new_connection/pop3.h"
#include <stdio.h>

void on_arrival_force_quit(const unsigned state, struct selector_key *key){
    struct connection_state * data = key->data;
    if (data->auth_data.user_index != -1){
        data->args->users[data->auth_data.user_index].close = false;
    }
    if (data->commands.pop3_current_command->cmd_id == RETR ){
        selector_unregister_fd(key->s, data->commands.pop3_current_command->retr_state.mail_fd);
    }
    metrics_register_client_disconnected();
    selector_unregister_fd(key->s, key->fd);
    key->data = NULL;

}
void on_departure_force_quit(const unsigned state, struct selector_key *key){
    ((struct connection_state *)key->data)->commands.last_state = FORCED_QUIT_STATE;
}
enum pop3_states on_read_ready_force_quit(struct selector_key *key){
     return AUTHORIZATION_STATE;
}
enum pop3_states on_write_ready_force_quit(struct selector_key *key){ return 0; }
enum pop3_states on_block_ready_force_quit(struct selector_key *key){ return 0;}
