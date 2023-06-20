#ifndef SHARED_HEADER 
#define SHARED_HEADER 


#include "../lib/buffer/buffer.h"
//#include "./commands/write_list.h"
#define BUFFER_SIZE 8192
#define POP3_MAX_CMD_LENGTH 10
#define POP3_MAX_ARG_LENGTH 41
#define POP3_MAX_EMAILS 500
#define NAME_MAX 1024
#define PATH_MAX 4096

enum monitor_states {
  AUTH_MONITOR,
  TRANSACTION_MONITOR,
  QUIT_MONITOR,
  FORCED_QUIT_MONITOR,
  ERROR_MONITOR,
  COUNT_STATES_MONITOR
};

enum MONITOR_CMD_ID {
  USERNAME,
  PASSWORD,
  METRICS,
  EXIT,
  ADD_USER,
  CAPA_MONITOR,
  LIST_USERS,
  MONITOR_CMD_COUNT
};

enum pop3_states {
    AUTHORIZATION_STATE = 0,
    TRANSACTION_STATE,
    UPDATE_STATE,
    ERROR_STATE,
    FORCED_QUIT_STATE,
    NONE_STATE,
    SERVER_STATE_COUNT,
};

struct authorization_data{
    int user_index;
    bool is_logged;
};

enum CMD_ID {
    USER = 0,
    PASS,
    CAPA_AUTH,
    QUIT_AUTH,
    QUIT_TRANSACTION,
    QUIT_UPDATE,
    LIST,
    RETR,
    DELE,
    NOOP,
    RSET,
    STAT,
    CAPA,
    CMD_ID_COUNT //siempre al final, mantiene la cantidad
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
typedef struct quit_update_state{
    bool has_deleted;
    bool has_error;
}quit_update_state;

enum metrics_ids {
  TOTAL_USERS = 1, TOTAL_RETRIEVED = 2, TOTAL_DELETED = 3, CURRENT_USERS = 4, MAX_USERS_HISTORY = 5, TOTAL_BYTES_TRANSFERED = 6
}; 

typedef struct pop3_current_command{
    union {
       retr_state retr_state;
       list_state list_state;
       quit_update_state quit_update_state;
       bool noop_state;
       enum metrics_ids metric_id;
    };
    unsigned int cmd_id;
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

    unsigned int last_state;
};

#endif
