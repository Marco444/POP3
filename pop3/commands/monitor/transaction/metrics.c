#include "../../../../lib/metrics/metrics.h"
#include "../../command_service.h"
#include "../../../states/write_buffer_helpers.h"
#include <stdio.h>

#define TOTAL_USERS "TOTAL_USERS"
#define CURRENT_USERS "CURRENT_USERS"
#define MAX_USERS_HISTORY "MAX_USERS"
#define OK "+"
#define MAX_SIZE_METRIC_RESP 50


enum monitor_states handle_monitor_metrics(struct commands_state * ctx,struct selector_key *key) {
  if(ctx->arg1_length == 0) {

  }

  ctx->pop3_current_command->cmd_id = USERNAME;
  ctx->pop3_current_command->is_finished = false;
  ctx->pop3_current_command->has_error = false;
  ctx->pop3_current_command->noop_state = true;
 
  return TRANSACTION_MONITOR;
}
enum monitor_states handle_write_metrics_monitor(struct selector_key *key, pop3_current_command *current_command, struct commands_state *commands) {

  char message[MAX_SIZE_METRIC_RESP];
  MetricsSnapshot metrics;
  getMetricsSnapshot(&metrics);

  if(strcmp(TOTAL_USERS, commands->arg1)) {
    sprintf(message, "+ %zu \r\n", metrics.totalConnectionCount);
    write_in_buffer_monitor(key, message, strlen(message), 0);
    current_command->is_finished = true;    
    if(write_in_fd_monitor(key))
        return AUTH_MONITOR;
    return TRANSACTION_MONITOR;
  } 

  if(strcmp(CURRENT_USERS, commands->arg1)) {
    sprintf(message, "+ %zu \r\n", metrics.currentConnectionCount);
    write_in_buffer_monitor(key, message, strlen(message), 0);
    current_command->is_finished = true;    
    if(write_in_fd_monitor(key))
        return AUTH_MONITOR;
  }

  if(strcmp(MAX_USERS_HISTORY, commands->arg1)) {
    sprintf(message, "+ %zu \r\n", metrics.maxConcurrentConnections);
    write_in_buffer_monitor(key, message, strlen(message), 0);
    current_command->is_finished = true;    
    if(write_in_fd_monitor(key))
        return AUTH_MONITOR;
  }
  return TRANSACTION_MONITOR;
}
