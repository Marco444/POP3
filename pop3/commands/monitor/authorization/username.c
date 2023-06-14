#include "../../command_service.h"
#include "../../../states/write_buffer_helpers.h"
#include <stdio.h>

#define OK_MSG "+ OK\r\n"

enum monitor_states handle_monitor_username(struct commands_state * ctx,struct selector_key *key) {
  puts("USERNAME");
  ctx->pop3_current_command->cmd_id = USERNAME;
  ctx->pop3_current_command->is_finished = false;
  ctx->pop3_current_command->has_error = false;
  ctx->pop3_current_command->noop_state = true;
  return AUTH_MONITOR;
}

enum monitor_states handle_write_username_monitor(struct selector_key *key, pop3_current_command *current_command, struct commands_state *commands) {
  write_in_buffer_monitor(key, OK_MSG, strlen(OK_MSG), 0);
  current_command->is_finished = true;
  if(write_in_fd_monitor(key))
      return AUTH_MONITOR;
  return ERROR_MONITOR;
}
