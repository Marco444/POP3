#include "../../monitor_states.h"
#include "../../pop3_states.h"
#include "commands.h"

// State function declarations
void on_arrival_auth_monitor(const unsigned state, struct selector_key *key){ return; }
void on_departure_auth_monitor(const unsigned state, struct selector_key *key){ return; }

enum monitor_states on_read_ready_auth_monitor(struct selector_key *key){ 
  return read_commands_monitor(key, AUTH_MONITOR, true); 
}

enum monitor_states on_write_ready_auth_monitor(struct selector_key *key){ return AUTH_MONITOR; }
enum monitor_states on_block_ready_auth_monitor(struct selector_key *key){ return AUTH_MONITOR; }