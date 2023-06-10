#include "../command_service.h"
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#define PATH_DIR "/home/lhernando/protos/foo/laucha/"

enum pop3_states handle_pass(struct commands_state * ctx, struct selector_key *key) {
    DIR *folder;
    struct dirent * entry;
    folder = opendir(PATH_DIR);
    if(folder == NULL)
    {
        puts("Unable to read directory");
        return(1);
    }
    struct stat st;
    int i = 0;
    while( (entry=readdir(folder)) && i < POP3_MAX_EMAILS)
    {
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;
        strcpy(ctx->email_files[i].name, entry->d_name);
        strcpy(ctx->email_files[i].path, PATH_DIR);
        strcat(ctx->email_files[i].path, entry->d_name);
        stat(ctx->email_files[i].path, &st);
        long size = (long)st.st_size;
        ctx->email_files[i].is_deleted = false; 
        ctx->email_files[i].size = size;
        i++;
    }
    ctx->email_files_length = i;
    ctx->pop3_current_command->cmd_id = PASS;
    ctx->pop3_current_command->is_finished = false;
    ctx->pop3_current_command->has_error = false;
    ctx->pop3_current_command->noop_state = true;
    return AUTHORIZATION_STATE;
}
