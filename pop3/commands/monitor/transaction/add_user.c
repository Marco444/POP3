// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../../command_service.h"
#include "../../../states/write_buffer_helpers.h"
#include <stdio.h>

#define OK_ADD_USER "+OK user added\r\n"
#define ERRORS_ADD_USERS "-ERR could not add user\r\n"

enum monitor_states handle_monitor_add_user(struct commands_state * ctx,struct selector_key *key) {
  puts("ADD_USER");
  ctx->pop3_current_command->cmd_id = ADD_USER;
  ctx->pop3_current_command->is_finished = false;
  ctx->pop3_current_command->has_error = false;
  ctx->pop3_current_command->noop_state = true;

  struct monitor_connection_state * state = (struct monitor_connection_state *) key->data;

  int i;
  
  // Cannot add more users, maximun size reached
  if(state->args->users_count >= MAX_USERS){
    ctx->pop3_current_command->has_error = true;
    return TRANSACTION_MONITOR;
  }

  

  char *p = strchr(ctx->arg1, ':');
  if(p == NULL) {
    ctx->pop3_current_command->has_error = true;
    return TRANSACTION_MONITOR;
  } else {
    *p = 0;
    p++;
    
    strcpy(state->args->users[state->args->users_count].name, ctx->arg1);
    strcpy(state->args->users[state->args->users_count].pass, p);
    
    state->args->users[state->args->users_count].close = false;
  }

  state->args->users_count += 1;
  ctx->pop3_current_command->has_error = false;
  return TRANSACTION_MONITOR;
}

enum monitor_states handle_write_add_user_monitor(struct selector_key *key, pop3_current_command *current_command, struct commands_state *commands) {
  
    if(!current_command->has_error){
    // Print OK message
    bool has_place = enters_the_buffer_monitor(key, OK_ADD_USER);
    if(has_place){
      long offset = write_in_buffer_monitor(key, OK_ADD_USER, strlen(OK_ADD_USER), 0);
      if(offset == -1){
        current_command->is_finished = true;
      }
    }
  }else{
    // Print Error message
    bool has_place = enters_the_buffer_monitor(key, ERRORS_ADD_USERS);
    if(has_place){
      long offset = write_in_buffer_monitor(key, ERRORS_ADD_USERS, strlen(ERRORS_ADD_USERS), 0);
      if(offset == -1){
          current_command->is_finished = true;
      }
    }
  }
  
  if(write_in_fd_monitor(key))
      return TRANSACTION_MONITOR;
  return ERROR_MONITOR;
  
}
