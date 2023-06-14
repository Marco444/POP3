#include "../../command_service.h"
#include "../../../states/write_buffer_helpers.h"
#include <stdio.h>

enum monitor_states handle_monitor_password(struct commands_state * ctx,struct selector_key *key) {
  return AUTH_MONITOR;
}
enum monitor_states handle_write_password_monitor(struct selector_key *key, pop3_current_command *current_command, struct commands_state *commands) {
  return AUTH_MONITOR;
}
