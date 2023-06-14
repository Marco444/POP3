#include "../../command_service.h"
#include "../../../states/write_buffer_helpers.h"
#include <stdio.h>

#define CAPA_MSG "arrived"

enum monitor_states handle_monitor_username(struct commands_state * ctx,struct selector_key *key) {
  puts("USERNAME");
  ctx->pop3_current_command->cmd_id = USERNAME;
  ctx->pop3_current_command->is_finished = false;
  ctx->pop3_current_command->has_error = false;
  ctx->pop3_current_command->noop_state = true;
  return AUTH_MONITOR;
}

enum monitor_states handle_write_username_monitor(struct selector_key *key, pop3_current_command *current_command, struct commands_state *commands) {
  puts("llegue a escribir paa");
  write_in_buffer(key, CAPA_MSG, strlen(CAPA_MSG), 0);
  current_command->is_finished = true;
  return AUTH_MONITOR;
}
