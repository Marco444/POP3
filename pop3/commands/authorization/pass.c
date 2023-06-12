#include "../command_service.h"
#include "../../states/write_buffer_helpers.h"

#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>

#define PATH_DIR "/foo/laucha/"
#define ERRORS_PASS "-ERR unable to lock mailbox\r\n"
#define OK_PASS "+OK mailbox locked and ready\r\n"

enum pop3_states handle_pass(struct commands_state * ctx, struct selector_key *key) {
    DIR *folder;
    struct dirent * entry;
    struct connection_state * state = (struct connection_state *) key->data;
    ctx->pop3_current_command->cmd_id = PASS;
    ctx->pop3_current_command->is_finished = false;
    ctx->pop3_current_command->has_error = false;
    ctx->pop3_current_command->noop_state = true;

    if(state->auth_data.user_index == -1 )
    {
        ctx->pop3_current_command->has_error = true;
        return AUTHORIZATION_STATE;
    }
    if(strcmp(state->args->users[state->auth_data.user_index].pass, ctx->arg1) != 0)
    {
        ctx->pop3_current_command->has_error = true;
        return AUTHORIZATION_STATE;
    }
    state->auth_data.is_logged = true;
    char * path = malloc(strlen(state->args->users[state->auth_data.user_index].name) + strlen(state->args->mail_dir) + 40);
    strcpy(path, state->args->mail_dir);
    strcat(path, state->args->users[state->auth_data.user_index].name);
    strcat(path, "/curl/");
    folder = opendir(path);
    if(folder == NULL)
    {
        ctx->pop3_current_command->has_error = true;
        return AUTHORIZATION_STATE;
    }
    struct stat st;
    int i = 0;
    while( (entry=readdir(folder)) && i < POP3_MAX_EMAILS)
    {
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        strcpy(ctx->inbox_data.email_files[i].name, entry->d_name);
        strcpy(ctx->inbox_data.email_files[i].path, path);
        strcat(ctx->inbox_data.email_files[i].path, entry->d_name);
        stat(ctx->inbox_data.email_files[i].path, &st);
        long size = (long)st.st_size;
        ctx->inbox_data.email_files[i].is_deleted = false;
        ctx->inbox_data.email_files[i].size = size;
        ctx->inbox_data.total_size += size;
        i++;
    }
    ctx->inbox_data.email_files_length = i;
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
