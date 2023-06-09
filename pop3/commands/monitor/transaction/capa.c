#include "../../command_service.h"
#include "../../../../lib/logger/logger.h"
#include "../../../states/write_buffer_helpers.h"
#include <stdio.h>

#define OK_CAPA_MONITOR "+OK Capability list follows\n\rMETRICS\nEXIT\nLIST_USERS\nADD_USER\nCAPA\nCHANGE_DIRECTORY\r\n"
#define ERRORS_CAPA_MONITOR "-ERR\r\n"

enum monitor_states handle_monitor_capa(struct commands_state * ctx,struct selector_key *key){
    log_debug("CAPA - MONITOR");
    ctx->pop3_current_command->cmd_id = CAPA_MONITOR;
    ctx->pop3_current_command->is_finished = false;
    ctx->pop3_current_command->has_error = false;
    ctx->pop3_current_command->noop_state = true;

    if(ctx->arg1_length != 0){
        ctx->pop3_current_command->has_error = true;
        return TRANSACTION_MONITOR;
    }

    return TRANSACTION_MONITOR;
}

enum monitor_states handle_write_capa_monitor(struct selector_key *key, pop3_current_command *current_command, struct commands_state *commands){
    log_debug("CAPA - MONITOR - WRITE");
    if(current_command->has_error){
        bool has_place = enters_the_buffer_monitor(key, ERRORS_CAPA_MONITOR);
        if(has_place){
            long offset = write_in_buffer_monitor(key, ERRORS_CAPA_MONITOR, strlen(ERRORS_CAPA_MONITOR), 0);
            if(offset == -1){
                current_command->is_finished = true;
            }
        }
    }
    else{
        write_in_buffer_monitor(key, OK_CAPA_MONITOR, strlen(OK_CAPA_MONITOR), 0);
        current_command->is_finished = true;
    }
    if(write_in_fd_monitor(key)){
        return TRANSACTION_MONITOR;
    }
        return QUIT_MONITOR;
}
