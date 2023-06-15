// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../../command_service.h"
#include "../../../states/write_buffer_helpers.h"
#include <stdio.h>


enum monitor_states handle_monitor_add_user(struct commands_state * ctx,struct selector_key *key) {
  return TRANSACTION_MONITOR;
}
enum monitor_states handle_write_add_user_monitor(struct selector_key *key, pop3_current_command *current_command, struct commands_state *commands) {
  return TRANSACTION_MONITOR;
}
