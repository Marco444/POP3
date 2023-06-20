// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../../../../lib/metrics/metrics.h"
#include "../../command_service.h"
#include "../../../states/write_buffer_helpers.h"
#include <stdio.h>



#define OK "+"
#define ERROR_MSG "- "

#define MAX_SIZE_METRIC_RESP 200


static enum monitor_states write_str_buffer(struct selector_key * key, char * message, pop3_current_command * current_command) {
    write_in_buffer_monitor(key, message, strlen(message), 0);
    current_command->is_finished = true;    
    if(write_in_fd_monitor(key))
        return TRANSACTION_MONITOR;
    return ERROR_MONITOR;
}

static enum monitor_states write_metric(size_t metric, struct selector_key * key, char * message, pop3_current_command * current_command, struct commands_state * commands) {
    sprintf(message, "+\n%zu\r\n", metric);
    return write_str_buffer(key, message, current_command);
} 


static enum monitor_states write_metrics(Metrics_snapshot metric, struct selector_key * key, char * message, pop3_current_command * current_command, struct commands_state * commands) {
  sprintf(message, "+\n1 %zu\n2 %zu\n3 %zu\n4 %zu\n5 %zu\n6 %zu\r\n",
         metric.total_connection_count, metric.total_mails_retrieved, metric.total_mails_deleted, metric.current_connection_count, metric.max_concurrent_connections, metric.total_bytes_transferred);
  return write_str_buffer(key, message, current_command);
}

enum monitor_states handle_monitor_metrics(struct commands_state * ctx,struct selector_key *key) {
  ctx->pop3_current_command->cmd_id = METRICS;
  ctx->pop3_current_command->is_finished = false;
  ctx->pop3_current_command->has_error = ctx->arg2_length > 0;
  if(ctx->arg1_length > 0){
      int num = atoi(ctx->arg1);
      if(num > 0 && num < 7){
        ctx->pop3_current_command->metric_id = atoi(ctx->arg1);
        return TRANSACTION_MONITOR;
      }else{
        ctx->pop3_current_command->has_error = true;
        return TRANSACTION_MONITOR;
      }
  }
  ctx->pop3_current_command->metric_id = -1;
  return TRANSACTION_MONITOR;
}
enum monitor_states handle_write_metrics_monitor(struct selector_key *key, pop3_current_command *current_command, struct commands_state *commands) {

  char message[MAX_SIZE_METRIC_RESP] = {0};
  Metrics_snapshot metrics;
  get_metrics_snapshot(&metrics);

  int arg = commands->pop3_current_command->metric_id;

  if(current_command->has_error)
    return write_str_buffer(key, ERROR_MSG, current_command);

  if(arg == TOTAL_USERS)
    return write_metric(metrics.total_connection_count, key, message, current_command, commands);

  if(arg == CURRENT_USERS) 
    return write_metric(metrics.current_connection_count, key, message, current_command, commands);

  if(arg == MAX_USERS_HISTORY) 
    return write_metric(metrics.max_concurrent_connections, key, message, current_command, commands);

  if(arg == TOTAL_DELETED) 
    return write_metric(metrics.total_mails_deleted, key, message, current_command, commands);

  if(arg == TOTAL_RETRIEVED) 
    return write_metric(metrics.total_mails_retrieved, key, message, current_command, commands);

  if(arg == TOTAL_BYTES_TRANSFERED) 
    return write_metric(metrics.total_bytes_transferred, key, message, current_command, commands);

  return write_metrics(metrics, key, message, current_command, commands);
}
