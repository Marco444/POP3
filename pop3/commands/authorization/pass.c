#include "../command_service.h"
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>

#define PATH_DIR "/home/lhernando/protos/foo/laucha/"

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

    folder = opendir(PATH_DIR);
    if(folder == NULL)
    {
        puts("Unable to read directory");
        return ERROR_STATE;
    }
    struct stat st;
    int i = 0;
    while( (entry=readdir(folder)) && i < POP3_MAX_EMAILS)
    {
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        strcpy(ctx->inbox_data.email_files[i].name, entry->d_name);
        strcpy(ctx->inbox_data.email_files[i].path, PATH_DIR);
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
