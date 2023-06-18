// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../../../../lib/metrics/metrics.h"
#include "../../command_service.h"
#include "../../../states/write_buffer_helpers.h"
#include <stdio.h>

static enum METRICS_ARGS {
  TOTAL_USERS = 0, TOTAL_RETRIEVED = 1, TOTAL_DELETED = 2, CURRENT_USERS = 3, MAX_USERS_HISTORY = 4
} args;

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
    sprintf(message, "+ %zu \r\n", metric);
    return write_str_buffer(key, message, current_command);
} 


static enum monitor_states write_metrics(MetricsSnapshot metric, struct selector_key * key, char * message, pop3_current_command * current_command, struct commands_state * commands) {
  sprintf(message, "+ total users: %zu \ncurrent users: %zu\nmax concurrent users handled: %zu\ntotal mails deleted: %zu\ntotal mails retrieved: %zu\r\n",
         metric.totalConnectionCount, metric.currentConnectionCount, metric.maxConcurrentConnections, metric.totalMailsDeleted, metric.totalMailsRetrieved );
  return write_str_buffer(key, message, current_command);
}

enum monitor_states handle_monitor_metrics(struct commands_state * ctx,struct selector_key *key) {
  ctx->pop3_current_command->cmd_id = METRICS;
  ctx->pop3_current_command->is_finished = false;
  ctx->pop3_current_command->has_error = ctx->arg2_length > 0;
  ctx->pop3_current_command->noop_state = true;

  return TRANSACTION_MONITOR;
}
enum monitor_states handle_write_metrics_monitor(struct selector_key *key, pop3_current_command *current_command, struct commands_state *commands) {

  char message[MAX_SIZE_METRIC_RESP] = {0};
  MetricsSnapshot metrics;
  getMetricsSnapshot(&metrics);
  printf("%s \n", commands->arg1);

  if(current_command->has_error)
    return write_str_buffer(key, ERROR_MSG, current_command);

  int arg = atoi(commands->arg1);

  printf("%d \n", arg);
  printf("%d \n", CURRENT_USERS);

  if(arg == TOTAL_USERS)
    return write_metric(metrics.totalConnectionCount, key, message, current_command, commands);

  if(arg == CURRENT_USERS) 
    return write_metric(metrics.currentConnectionCount, key, message, current_command, commands);

  if(arg == MAX_USERS_HISTORY) 
    return write_metric(metrics.maxConcurrentConnections, key, message, current_command, commands);

  if(arg == TOTAL_DELETED) 
    return write_metric(metrics.totalMailsDeleted, key, message, current_command, commands);

  if(arg == TOTAL_RETRIEVED) 
    return write_metric(metrics.totalMailsRetrieved, key, message, current_command, commands);

  return write_metrics(metrics, key, message, current_command, commands);
}
