#include "../../monitor_states.h"
void on_arrival_error_monitor(const unsigned state, struct selector_key *key) {return; }
void on_departure_error_monitor(const unsigned state, struct selector_key *key){return; }
enum monitor_states on_read_ready_error_monitor(struct selector_key *key){ return ERROR_MONITOR;}
enum monitor_states on_write_ready_error_monitor(struct selector_key *key) { return ERROR_MONITOR;}
enum monitor_states on_block_ready_error_monitor(struct selector_key *key){ return ERROR_MONITOR;}
