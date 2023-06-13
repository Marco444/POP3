#include "../../command_service.h"
#include <stdio.h>

enum monitor_states handle_monitor_username(struct commands_state * ctx,struct selector_key *key) {
  puts("llegue al auth del monitor");
  return AUTH_MONITOR;
}
