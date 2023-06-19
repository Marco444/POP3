// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../../monitor_states.h"

void on_arrival_quit_monitor(const unsigned state, struct selector_key *key) {
    selector_unregister_fd(key->s, key->fd);
    key->data = NULL;    
}

void on_departure_quit_monitor(const unsigned state, struct selector_key *key) {
    ((struct monitor_connection_state *)key->data)->commands.last_state = QUIT_MONITOR;
}
enum monitor_states on_read_ready_quit_monitor(struct selector_key *key) {return QUIT_MONITOR; }
enum monitor_states on_write_ready_quit_monitor(struct selector_key *key){return QUIT_MONITOR; }
enum monitor_states on_block_ready_quit_monitor(struct selector_key *key){return QUIT_MONITOR; }
