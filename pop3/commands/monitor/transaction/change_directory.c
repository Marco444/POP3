#include "../../command_service.h"
#include "../../../states/write_buffer_helpers.h"
#include "../../../../lib/logger/logger.h"
#include "../../../shared.h"
#include <stdio.h>
#include <sys/stat.h>


#define OK_CHANGE_DIRECTORY "+\r\n"
#define ERROS "-\r\n"

enum monitor_states handle_monitor_change_directory(struct commands_state * ctx,struct selector_key *key) {
  ctx->pop3_current_command->cmd_id = CHANGE_DIRECTORY;
  ctx->pop3_current_command->is_finished = false;
  ctx->pop3_current_command->has_error = false;
  ctx->pop3_current_command->noop_state = true;

  struct monitor_connection_state * state = (struct monitor_connection_state *) key->data;
  if(ctx->arg1[0] == '\0'){
    ctx->pop3_current_command->has_error = true;
    return TRANSACTION_MONITOR;
  }
  if(ctx->arg1_length > PATH_MAX){
    ctx->pop3_current_command->has_error = true;
    return TRANSACTION_MONITOR;
  }
  strcpy(state->args->mail_dir, ctx->arg1);
  return TRANSACTION_MONITOR;
}

enum monitor_states handle_write_change_directory_monitor(struct selector_key *key, pop3_current_command *current_command, struct commands_state *commands) {
  
    if(!current_command->has_error){
    // Print OK message
    bool has_place = enters_the_buffer_monitor(key, OK_CHANGE_DIRECTORY);
    if(has_place){
      long offset = write_in_buffer_monitor(key, OK_CHANGE_DIRECTORY, strlen(OK_CHANGE_DIRECTORY), 0);
      if(offset == -1){
        current_command->is_finished = true;
      }
    }
  }else{
    // Print Error message
    bool has_place = enters_the_buffer_monitor(key, ERROS);
    if(has_place){
      long offset = write_in_buffer_monitor(key, ERROS, strlen(ERROS), 0);
      if(offset == -1){
          current_command->is_finished = true;
      }
    }
  }
  
  if(write_in_fd_monitor(key))
      return TRANSACTION_MONITOR;
  return ERROR_MONITOR;
  
}
