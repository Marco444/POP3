// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../../monitor_states.h"
#include "../../pop3_states.h"
#include "commands.h"

// State function declarations for TRANSACTION_STATE
void on_arrival_trans_monitor(const unsigned state, struct selector_key *key) { return; }

void on_departure_trans_monitor(const unsigned state, struct selector_key *key) {
    ((struct monitor_connection_state *)key->data)->commands.last_state = TRANSACTION_MONITOR;
}

enum monitor_states on_read_ready_trans_monitor(struct selector_key *key) {
  return read_commands_monitor(key, TRANSACTION_MONITOR, true); 
}

enum monitor_states on_write_ready_trans_monitor(struct selector_key *key){
  return write_command_monitor(key); 
}

enum monitor_states on_block_ready_trans_monitor(struct selector_key *key){ return TRANSACTION_MONITOR; }
