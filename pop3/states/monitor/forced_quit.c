// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "../../monitor_states.h"
#include "../../pop3_states.h"
#include "commands.h"

void on_arrival_forced_quit_monitor(const unsigned state, struct selector_key *key) { return;}
void on_departure_forced_quit_monitor(const unsigned state, struct selector_key *key){ return;}
enum monitor_states on_read_ready_forced_quit_monitor(struct selector_key *key){
  return FORCED_QUIT_MONITOR;
}
enum monitor_states on_write_ready_forced_quit_monitor(struct selector_key *key){
  return FORCED_QUIT_MONITOR;
}
enum monitor_states on_block_ready_forced_quit_monitor(struct selector_key *key){
  return FORCED_QUIT_MONITOR;
}
