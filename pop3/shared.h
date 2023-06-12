#ifndef SHARED_HEADER 
#define SHARED_HEADER 


#include "../lib/buffer/buffer.h"
//#include "./commands/write_list.h"
#define BUFFER_SIZE 4096
#define POP3_MAX_CMD_LENGTH 512 
#define POP3_MAX_ARG_LENGTH 512 
#define POP3_MAX_EMAILS 500
#define NAME_MAX 1024
#define PATH_MAX 1024
enum pop3_states {
    AUTHORIZATION_STATE = 0,
    TRANSACTION_STATE,
    UPDATE_STATE,
    ERROR_STATE,
    FORCED_QUIT_STATE,
    SERVER_STATE_COUNT, 
} ;
struct authorization_data{
    int user_index;
    bool is_logged;
};
enum CMD_ID {
    USER,
    PASS,
    QUIT,
    LIST,
    RETR,
    DELE,
    NOOP,
    RSET,
    STAT,
};
typedef struct email_file{
    char name[NAME_MAX];
    char path[PATH_MAX];
    bool is_deleted;
    long size;
} email_file;
typedef struct inbox_data{
    email_file email_files[POP3_MAX_EMAILS];
    int email_files_length;
    long total_size;
} inbox_data;

typedef struct retr_state{
    bool mail_finished;
    int mail_fd;
    bool title_sent;
}retr_state;
typedef struct list_state{
    bool title_sent;
    int current_index;
    int argument;

}list_state;
typedef struct pop3_current_command{
    union {
       retr_state retr_state;
       list_state list_state;
       bool noop_state;
    };
    enum CMD_ID cmd_id;
    bool is_finished;
    bool has_error;
}pop3_current_command;

struct commands_state {
    buffer read_buffer; 
    buffer write_buffer;

    pop3_current_command * pop3_current_command;
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
    inbox_data inbox_data;
    int email_fd;
};

#endif
