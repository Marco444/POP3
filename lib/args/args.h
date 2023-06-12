#ifndef ARGS_H_kFlmYm1tW9p5npzDr2opQJ9jM8
#define ARGS_H_kFlmYm1tW9p5npzDr2opQJ9jM8

#include <stdbool.h>

#define MAX_USERS 10

struct users {
    char *name;
    char *pass;
    bool close;

};

typedef struct user {
    char * name;
    char * pass;
    bool close;
} user;


// struct pop3args {
//     char           *pop3_addr;
//     unsigned short  pop3_port;

// };

typedef struct pop3args {
    // USERS (Array is null terminated)
    struct user users[MAX_USERS];
    // POP3 server Address and port
    char           * pop3_addr;
    unsigned short  pop3_port;
    // Monitor address and port
    char            *monitor_addr;
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

