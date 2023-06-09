// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../../command_service.h"
#include "../../../states/write_buffer_helpers.h"
#include <stdio.h>
int write_in_fd(struct selector_key *key);


#define MAX_OUTPUT 100
#define OK_STAT_START "+OK"
#define OK_STAT_END "\r\n"
#define ERRORS_STAT "-ERR\r\n"

enum pop3_states handle_stat(struct commands_state * ctx, struct selector_key *key) {
     ctx->pop3_current_command->cmd_id = STAT;
    ctx->pop3_current_command->is_finished = false;
    ctx->pop3_current_command->has_error = false;
    ctx->pop3_current_command->noop_state = true;
    if (ctx->arg1_length != 0) {
        ctx->pop3_current_command->has_error = true;
        return TRANSACTION_STATE;
    }
    return TRANSACTION_STATE;
}

enum pop3_states handle_write_stat(struct selector_key *key, pop3_current_command *current_command, struct commands_state *commands) {

    if(current_command->has_error){
        bool has_place = enters_the_buffer(key, ERRORS_STAT);
        if(has_place) {
            long offset = write_in_buffer(key, ERRORS_STAT, strlen(ERRORS_STAT), 0);
            if (offset == -1) {
                current_command->is_finished = true;
            }
        }
    }else{
        int emails_count = 0;
        int emails_total_size = 0;

        int i;
        for(i = 0; i < commands->inbox_data.email_files_length; i++){

            if(!commands->inbox_data.email_files[i].is_deleted){
                emails_total_size += commands->inbox_data.email_files[i].size;
                emails_count++;
            }

        }
        char output[MAX_OUTPUT];
        sprintf(output, "%s %d %d %s", OK_STAT_START, emails_count, emails_total_size, OK_STAT_END);
        bool has_place = enters_the_buffer(key, output);
        if(has_place){
            long offset = write_in_buffer(key, output, strlen(output), 0);
            if(offset == -1){
                current_command->is_finished = true;
            }
        }
    }
    if(write_in_fd(key)) {
        return TRANSACTION_STATE;
    }else{
        return FORCED_QUIT_STATE;
    }
}
