// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../../../../lib/logger/logger.h"
#include "../../command_service.h"
#include "../../../states/write_buffer_helpers.h"
#include <stdio.h>

#define OK_MSG "+ OK\r\n"
#define ERROR_MSG "- ERR\r\n"

enum monitor_states handle_monitor_password(struct commands_state * ctx,struct selector_key *key) {
  log_debug("PASSWORD");

  struct monitor_connection_state * state = (struct monitor_connection_state *) key->data;

  ctx->pop3_current_command->cmd_id = PASSWORD;
  ctx->pop3_current_command->is_finished = false;
  ctx->pop3_current_command->has_error = false;
  ctx->pop3_current_command->noop_state = true;
  
  // In case we have not recieved a USERNAME command before
  if(state->auth_data.user_index == -1 ){
    ctx->pop3_current_command->has_error = true;
    return AUTH_MONITOR;
  }

  if(strcmp(state->args->users_admin[state->auth_data.user_index].pass, ctx->arg1) != 0){
    ctx->pop3_current_command->has_error = true;
    return AUTH_MONITOR;
  }
  
  if(state->args->users_admin[state->auth_data.user_index].close == true){
    ctx->pop3_current_command->has_error = true;
    return AUTH_MONITOR;
  }

  return AUTH_MONITOR;
}

enum monitor_states handle_write_password_monitor(struct selector_key *key, pop3_current_command *current_command, struct commands_state *commands) {
  log_debug("PASSWORD write answer");
  
  if(!current_command->has_error){
    bool has_place = enters_the_buffer_monitor(key, OK_MSG);
    if(has_place){
      long offset = write_in_buffer_monitor(key, OK_MSG, strlen(OK_MSG), 0);
      if(offset == -1){
        current_command->is_finished = true;
      }
    }
  }else{
    bool has_place = enters_the_buffer_monitor(key, ERROR_MSG);
    if(has_place){
      long offset = write_in_buffer_monitor(key, ERROR_MSG, strlen(ERROR_MSG), 0);
      if(offset == -1){
        current_command->is_finished = true;
      }
    }
  }

  if(write_in_fd_monitor(key)){
    if(current_command->has_error)
      return AUTH_MONITOR;
    else
      return TRANSACTION_MONITOR;
    
  }else
      return FORCED_QUIT_MONITOR;
    

}
