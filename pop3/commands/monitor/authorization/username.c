// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../../../../lib/logger/logger.h"
#include "../../command_service.h"
#include "../../../states/write_buffer_helpers.h"
#include <stdio.h>

#define OK_ADMIN_USER "+OK is a valid admin user\r\n"
#define ERRORS_ADMIN_USER "-ERR never heard of admin name\r\n"

enum monitor_states handle_monitor_username(struct commands_state * ctx,struct selector_key *key) {
  log_debug("USERNAME");
  ctx->pop3_current_command->cmd_id = USERNAME;
  ctx->pop3_current_command->is_finished = false;
  ctx->pop3_current_command->has_error = false;
  ctx->pop3_current_command->noop_state = true;
 
  struct monitor_connection_state * state = (struct monitor_connection_state *) key->data;
  
  int i;
  for(i = 0; state->args->users_admin[i].name[0] != '\0' && i < state->args->users_admin_count; i++){
    if(strcmp(state->args->users_admin[i].name, ctx->arg1) == 0){
      state->auth_data.user_index = i;
      state->auth_data.is_logged = false;
      return AUTH_MONITOR; 
    }
  }

  state->auth_data.user_index = -1;
  state->auth_data.is_logged = false;
  ctx->pop3_current_command->has_error = true;
  return AUTH_MONITOR;
}

enum monitor_states handle_write_username_monitor(struct selector_key *key, pop3_current_command *current_command, struct commands_state *commands) {
  if(!current_command->has_error){
    // Print OK message
    bool has_place = enters_the_buffer_monitor(key, OK_ADMIN_USER);
    if(has_place){
      long offset = write_in_buffer_monitor(key, OK_ADMIN_USER, strlen(OK_ADMIN_USER), 0);
      if(offset == -1){
        current_command->is_finished = true;
      }
    }
  }else{
    // Print Error message
    bool has_place = enters_the_buffer_monitor(key, ERRORS_ADMIN_USER);
    if(has_place){
      long offset = write_in_buffer_monitor(key, ERRORS_ADMIN_USER, strlen(ERRORS_ADMIN_USER), 0);
      if(offset == -1){
          current_command->is_finished = true;
      }
    }
  }
  
  if(write_in_fd_monitor(key))
      return AUTH_MONITOR;
  return ERROR_MONITOR;
}
