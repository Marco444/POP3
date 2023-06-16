// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../../../../lib/metrics/metrics.h"
#include "../../command_service.h"
#include "../../../states/write_buffer_helpers.h"
#include <stdio.h>

#define TOTAL_USERS "TOTAL_USERS"
#define TOTAL_RETRIEVED "TOTAL_RETRIEVED_MAILS"
#define TOTAL_DELETED "TOTAL_DELETED_MAILS"
#define CURRENT_USERS "CURRENT_USERS"
#define MAX_USERS_HISTORY "MAX_USERS"

#define OK "+"
#define ERROR_MSG "- Invalid arguments "

#define MAX_SIZE_METRIC_RESP 50


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

enum monitor_states handle_monitor_metrics(struct commands_state * ctx,struct selector_key *key) {
  puts("METRICS");

  ctx->pop3_current_command->cmd_id = METRICS;
  ctx->pop3_current_command->is_finished = false;
  ctx->pop3_current_command->has_error = ctx->arg1_length == 0 || ctx->arg2_length > 0;
  ctx->pop3_current_command->noop_state = true;

  return TRANSACTION_MONITOR;
}
enum monitor_states handle_write_metrics_monitor(struct selector_key *key, pop3_current_command *current_command, struct commands_state *commands) {

  char message[MAX_SIZE_METRIC_RESP] = {0};
  MetricsSnapshot metrics;
  getMetricsSnapshot(&metrics);

  printf("arg1: %s \n", commands->arg1);

  if(current_command->has_error)
    return write_str_buffer(key, ERROR_MSG, current_command);

  if(strcmp(TOTAL_USERS, commands->arg1) == 0) {
    puts("TOTAL_USERS");
    return write_metric(metrics.totalConnectionCount, key, message, current_command, commands);
  }

  if(strcmp(CURRENT_USERS, commands->arg1) == 0) 
    return write_metric(metrics.currentConnectionCount, key, message, current_command, commands);

  if(strcmp(MAX_USERS_HISTORY, commands->arg1) == 0)
    return write_metric(metrics.maxConcurrentConnections, key, message, current_command, commands);

  if(strcmp(TOTAL_DELETED, commands->arg1) == 0) {
    puts("TOTAL_DELETED");
    return write_metric(metrics.totalMailsDeleted, key, message, current_command, commands);
  }

  if(strcmp(TOTAL_RETRIEVED, commands->arg1) == 0)
    return write_metric(metrics.totalMailsRetrieved, key, message, current_command, commands);

  return TRANSACTION_MONITOR;
}
