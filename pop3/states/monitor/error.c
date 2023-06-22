#include "../../monitor_states.h"
#include "../../pop3_states.h"
#include "commands.h"
#include "../write_buffer_helpers.h"


#define ERROR_MSG "-ERR Invalid command \n"

void on_arrival_error_monitor(const unsigned state, struct selector_key *key) {
  return; 
}
void on_departure_error_monitor(const unsigned state, struct selector_key *key){
    ((struct monitor_connection_state *)key->data)->commands.last_state = ERROR_MONITOR;
}
enum monitor_states on_read_ready_error_monitor(struct selector_key *key){ return ERROR_MONITOR;}

enum monitor_states on_write_ready_error_monitor(struct selector_key *key) { 
    write_in_buffer_monitor(key, ERROR_MSG, strlen(ERROR_MSG), 0);
    write_in_fd_monitor(key);
    return ((struct monitor_connection_state *)key->data)->commands.last_state;

}
enum monitor_states on_block_ready_error_monitor(struct selector_key *key){ return ERROR_MONITOR;}
