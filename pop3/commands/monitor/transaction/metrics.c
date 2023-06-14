
#include "../../command_service.h"
#include "../../../states/write_buffer_helpers.h"
#include <stdio.h>

enum monitor_states handle_monitor_metrics(struct commands_state * ctx,struct selector_key *key) {
  return TRANSACTION_MONITOR;
}
enum monitor_states handle_write_metrics_monitor(struct selector_key *key, pop3_current_command *current_command, struct commands_state *commands) {
  return TRANSACTION_MONITOR;
}
