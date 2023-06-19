// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../../command_service.h"
#include "../../../states/write_buffer_helpers.h"
#include "../../../../lib/logger/logger.h"

#define QUIT_MSG_TRANSACTION "+OK server signing off\r\n"

enum monitor_states handle_monitor_exit(struct commands_state * ctx,struct selector_key *key) {
  log_debug("EXIT read");
    ctx->pop3_current_command->cmd_id = EXIT;
    ctx->pop3_current_command->is_finished = false;
    ctx->pop3_current_command->has_error = false;
    ctx->pop3_current_command->noop_state = true;
    struct monitor_connection_state * data = key->data;
    
    if (data->auth_data.user_index != -1){
        data->args->users[data->auth_data.user_index].close = false;
    }
    
  return TRANSACTION_MONITOR;
}
enum monitor_states handle_write_exit_monitor(struct selector_key *key, pop3_current_command *current_command, struct commands_state *commands) {
  write_in_buffer_monitor(key, QUIT_MSG_TRANSACTION, strlen(QUIT_MSG_TRANSACTION), 0);
  current_command->is_finished = true;    
  
  if(write_in_fd_monitor(key))
      return QUIT_MONITOR;
  
  return ERROR_MONITOR;
}
