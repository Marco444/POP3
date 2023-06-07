#ifndef SHARED_HEADER 
#define SHARED_HEADER 


#include "../lib/buffer/buffer.h"
#include "./commands/write_list.h"
#include <linux/limits.h>
#define BUFFER_SIZE 4096
#define POP3_MAX_CMD_LENGTH 512 
#define POP3_MAX_ARG_LENGTH 512 
#define POP3_MAX_EMAILS 500
enum pop3_states {
    AUTHORIZATION_STATE = 0,
    TRANSACTION_STATE,
    UPDATE_STATE,
    ERROR_STATE,
    FORCED_QUIT_STATE,
    SERVER_STATE_COUNT, 
} ;
typedef struct email_file{
    char name[NAME_MAX];
    char path[PATH_MAX];
    bool is_deleted;
    long size;
} email_file;
struct commands_state {
    buffer read_buffer; 
    buffer write_buffer;

    elem_type  write_data; 
    
    //donde almaceno la informacion del ADT buffer 
    uint8_t in_buffer[BUFFER_SIZE+1];
    uint8_t out_buffer[BUFFER_SIZE+1];
    size_t in_buffer_length, out_buffer_length;

    //donde almaceno el comando y argumentos que rellena el parser
    //a partir de los buffers
    char cmd[POP3_MAX_CMD_LENGTH + 1];
    char arg1[POP3_MAX_ARG_LENGTH + 1];
    char arg2[POP3_MAX_ARG_LENGTH + 1];
    size_t cmd_length, arg1_length, arg2_length;

    // Aca va a tener la lista de los archivos que tiene en el file
    email_file  email_files[POP3_MAX_EMAILS];
    int email_files_length;
    int email_fd;
};

#endif
