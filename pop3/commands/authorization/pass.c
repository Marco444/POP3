#include "../command_service.h"
#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#define PATH_DIR "/home/laucha/protos/POP3/foo/laucha"

enum pop3_states handle_pass(struct commands_state * ctx) {
 elem_type elem = calloc(1,sizeof(struct cmd));
    DIR *folder;
    struct dirent *entry;

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

        stat(entry->d_name, &st);
        if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            continue;

        strcpy(ctx->email_files[i].name, entry->d_name);
        strcpy(ctx->email_files[i].path, PATH_DIR);
        strcat(ctx->email_files[i].path, entry->d_name);    
        ctx->email_files[i].is_deleted = false; 
        ctx->email_files[i].size = st.st_size;
        i++;
    }
    ctx->email_files_length = i;

    elem->cmd_id = PASS;
    elem->offset = 0;
    elem->is_done = false;
    ctx->write_data = elem;
    return AUTHORIZATION_STATE; 
}
