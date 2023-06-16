
#ifndef PROTOS_ARGS_H
#define PROTOS_ARGS_H


#include <stdbool.h>

#define MAX_ARG_LENGTH 255

typedef struct user {
    char *name;
    char *pass;
}user;

typedef struct command{
    int index;
    char arg[MAX_ARG_LENGTH];
}command;
typedef struct server_data{
    char *server_addr;
    unsigned short server_port;
}server_data;

typedef struct client_args {
    // USERS (Array is null terminated)
     user user;

     server_data server_data;

    // POP3 server Address and port
    command command;
} client_args;


/**
 * Interpreta la linea de comandos (argc, argv) llenando
 * args con defaults o la seleccion humana. Puede cortar
 * la ejecución.
 */
void
parse_args(const int argc, char **argv, struct client_args *args);



#endif
