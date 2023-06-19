#ifndef ARGS_H_kFlmYm1tW9p5npzDr2opQJ9jM8
#define ARGS_H_kFlmYm1tW9p5npzDr2opQJ9jM8

#include <stdbool.h>

#define MAX_USERS 10
#define MAX_USERS_NAME_LENGHT 10
#define MAX_USERS_PASS_LENGHT 10
#define SOCKETS_DATA 2

struct users {
    char name[MAX_USERS_NAME_LENGHT];
    char pass[MAX_USERS_PASS_LENGHT];
    bool close;

};

typedef struct user {
    char name[MAX_USERS_NAME_LENGHT];
    char pass[MAX_USERS_PASS_LENGHT];
    bool close;
} user;

typedef struct conection_data
{

    char * pop3_addr;
    unsigned short pop3_port;
}conection_data;


typedef struct pop3args {
    // USERS (Array is null terminated)
    int users_count;
    int users_admin_count;
    struct user users[MAX_USERS];
    struct user users_admin[MAX_USERS];
    
    conection_data conection_data[SOCKETS_DATA];

    conection_data conection_data_monitor[SOCKETS_DATA];
    
    // Monitor address and port
    char        * monitor_addr;
    unsigned short  monitor_port;
    // Directory
    char            *mail_dir;
} pop3args;


/**
 * Interpreta la linea de comandos (argc, argv) llenando
 * args con defaults o la seleccion humana. Puede cortar
 * la ejecución.
 */
void 
parse_args(const int argc, char **argv, struct pop3args *args);

#endif

