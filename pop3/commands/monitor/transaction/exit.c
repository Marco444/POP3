// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../../command_service.h"
#include "../../../states/write_buffer_helpers.h"
#include <stdio.h>

enum monitor_states handle_monitor_exit(struct commands_state * ctx,struct selector_key *key) {
    puts("EXIT");
    ctx->pop3_current_command->cmd_id = EXIT;
    ctx->pop3_current_command->is_finished = false;
    ctx->pop3_current_command->has_error = false;
    ctx->pop3_current_command->noop_state = true;
    struct connection_state * data = key->data;
    if (data->auth_data.user_index != -1){
        data->args->users[data->auth_data.user_index].close = false;
    }
  return AUTH_MONITOR;
}
enum monitor_states handle_write_exit_monitor(struct selector_key *key, pop3_current_command *current_command, struct commands_state *commands) {
  return AUTH_MONITOR;
}
