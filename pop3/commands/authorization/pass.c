#include "../command_service.h"
#include "../../states/write_buffer_helpers.h"

#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>

#define PATH_DIR "/foo/passwords"

#define ERRORS_PASS "-ERR unable to lock mailbox\r\n"
#define OK_PASS "+OK mailbox locked and ready\r\n"

enum pop3_states handle_pass(struct commands_state * ctx, struct selector_key *key) {
    // DIR *folder;
    // struct dirent * entry;
    // folder = opendir(PATH_DIR);
    // if(folder == NULL)
    // {
    //     puts("Unable to read directory");
    //     return ERROR_STATE;
    // }
    // struct stat st;
    // int i = 0;
    // while( (entry=readdir(folder)) && i < POP3_MAX_EMAILS)
    // {
    //     if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
    //         continue;
    //     strcpy(ctx->email_files[i].name, entry->d_name);
    //     strcpy(ctx->email_files[i].path, PATH_DIR);
    //     strcat(ctx->email_files[i].path, entry->d_name);
    //     stat(ctx->email_files[i].path, &st);
    //     long size = (long)st.st_size;
    //     ctx->email_files[i].is_deleted = false; 
    //     ctx->email_files[i].size = size;
    //     i++;
    // }
    //ctx->email_files_length = i;
    ctx->pop3_current_command->cmd_id = PASS;
    ctx->pop3_current_command->is_finished = false;
    ctx->pop3_current_command->has_error = false;
    ctx->pop3_current_command->noop_state = true;
    return AUTHORIZATION_STATE;
}


enum pop3_states handle_write_pass(struct selector_key *key, pop3_current_command * pop3_current, struct commands_state *commands) {
    if (!pop3_current->has_error) {
        bool has_place = enters_the_buffer(key, OK_PASS);
        if (has_place) {
            long offset = write_in_buffer(key, OK_PASS, strlen(OK_PASS), 0);
            if (offset == -1) {
                pop3_current->is_finished = true;
            }
        }
    } else {
        bool has_place = enters_the_buffer(key, ERRORS_PASS);
        if (has_place) {
            long offset = write_in_buffer(key, ERRORS_PASS, strlen(ERRORS_PASS), 0);
            if (offset == -1) {
                pop3_current->is_finished = true;
            }
        }
    }
    return TRANSACTION_STATE;
}
