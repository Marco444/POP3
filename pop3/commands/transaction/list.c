#include "../command_service.h"
#include "../../../lib/selector/selector.h"
#include "../../states/write_buffer_helpers.h"
#include <stdio.h>

#define OK_LIST "+OK scan list follows\r\n"
#define ERRORS_QUIT "-ERR Quit\r\n"
#define OK_QUIT "+OK Quit\r\n"
#define ERRORS_LIST "-ERR no such message\r\n"
#define FINISH_LIST "\r\n.\r\n"

enum pop3_states handle_list(struct commands_state * ctx, struct selector_key *key) {
    ctx->pop3_current_command->cmd_id = LIST;
    ctx->pop3_current_command->is_finished = false;
    ctx->pop3_current_command->has_error = false;
    ctx->pop3_current_command->list_state.title_sent = false;
    ctx->pop3_current_command->list_state.current_index = 0;
    ctx->pop3_current_command->list_state.argument = -1;
    if(strlen(ctx->arg1) != 0)
    {
        if (atoi(ctx->arg1) - 1 >= ctx->email_files_length && atoi(ctx->arg1) <= 0){
            ctx->pop3_current_command->has_error = true;
            return TRANSACTION_STATE;
        }
        ctx->pop3_current_command->list_state.argument = atoi(ctx->arg1) - 1;
    }

    return TRANSACTION_STATE;
}

enum pop3_states handle_write_list(struct selector_key *key, pop3_current_command *current_command, struct commands_state *commands) {
    if (current_command->has_error) {
        bool has_place = enters_the_buffer(key, ERRORS_LIST);
        if (has_place) {
            long offset = write_in_buffer(key, ERRORS_LIST, strlen(ERRORS_LIST), 0);
            if (offset == -1) {
                current_command->is_finished = true;
            }
        }
    } else {
        if (!current_command->list_state.title_sent) {
            bool has_place = enters_the_buffer(key, OK_LIST);
            if (has_place) {
                long offset = write_in_buffer(key, OK_LIST, strlen(OK_LIST), 0);
                if (offset == -1) {
                    current_command->list_state.title_sent = true;
                }
            }
        }
        if (current_command->list_state.argument != -1){
            char buff[30];
            sprintf(buff, "%d %ld\r\n", current_command->list_state.argument + 1, commands->email_files[current_command->list_state.argument].size);
            bool has_place = enters_the_buffer(key, buff);
            if (has_place) {
                long offset = write_in_buffer(key, buff, strlen(buff), 0);
                if (offset == -1) {
                    current_command->is_finished = true;
                }
            }
        }else {
            for (int i = current_command->list_state.current_index; i < commands->email_files_length; ++i) {
                if (!commands->email_files[i].is_deleted) {
                    char buff2[30];
                    sprintf(buff2, "%d %ld\r\n", i + 1, commands->email_files[i].size);
                    bool has_place = enters_the_buffer(key, buff2);
                    if (has_place) {
                        long offset = write_in_buffer(key, buff2, strlen(buff2), 0);
                        if (offset == -1) {
                            current_command->list_state.current_index = i + 1;
                        }
                    } else {
                        current_command->list_state.current_index = i;
                        current_command->is_finished = false;
                        break;
                    }
                }
            }
            if (current_command->list_state.current_index == commands->email_files_length) {
                bool has_place = enters_the_buffer(key, FINISH_LIST);
                if (has_place) {
                    long offset = write_in_buffer(key, FINISH_LIST, strlen(FINISH_LIST), 0);
                    if (offset == -1) {
                        current_command->is_finished = true;
                    }
                }
            }
        }
    }
    return TRANSACTION_STATE;
}
