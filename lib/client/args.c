// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdio.h>     /* for printf */
#include <stdlib.h>    /* for exit */
#include <limits.h>    /* LONG_MIN et al */
#include <string.h>    /* memset */
#include <errno.h>
#include <getopt.h>

#include "args.h"

static unsigned short
port(const char *s) {
    char *end     = 0;
    const long sl = strtol(s, &end, 10);

    if (end == s|| '\0' != *end
        || ((LONG_MIN == sl || LONG_MAX == sl) && ERANGE == errno)
        || sl < 0 || sl > USHRT_MAX) {
        fprintf(stderr, "port should in in the range of 1-65536: %s\n", s);
        exit(1);
        return 1;
    }
    return (unsigned short)sl;
}

static void
write_user(char *s, struct user *user) {
    char *p = strchr(s, ':');
    if(p == NULL) {
        fprintf(stderr, "password not found\n");
        exit(1);
    } else {
        *p = 0;
        p++;
        user->name = s;
        user->pass = p;
    }

}

static void
version(void) {
    fprintf(stderr, "pop3 version 0.0\n"
                    "ITBA Protocolos de Comunicación 2023a \n");
}

void
parse_args(const int argc, char **argv, struct client_args *args) {
    memset(args, 0, sizeof(*args));

    // Default values for POP3 server address and port
    args->server_data.server_addr = "127.0.0.1";
    args->server_data.server_port = 8114;
    args->command.index = -1;

    char c;
    int nusers = 0;
    int nusers_admin = 0;

    // Flags for mandatory parameters
    bool mandatory_address = false;
    bool mandatory_port = false;

    while (true) {
        int option_index = 0;
        static struct option long_options[] = {
                { 0,           0,                 0, 0 }
        };
        c = getopt_long(argc, argv, "a:p:u:n:m:d:l", long_options, &option_index);

        if (c == -1)
            break;

        switch (c) {
            case 'a':
                args->server_data.server_addr = optarg;
                mandatory_address = true;
                break;
            case 'p':
                args->server_data.server_port = port(optarg);
                mandatory_port = true;
                break;
            case 'u':
                if(nusers == 1) {
                    fprintf(stderr, "cannot log in with more than 1 user\n");
                    exit(1);
                } else {
                    write_user(optarg, &args->user);
                    nusers++;
                }
                break;
            case 'n':
                if(args->command.index != -1) {
                    fprintf(stderr, "cannot use -n with -m\n");
                    exit(1);
                }
                strcpy(args->command.arg, optarg);
                args->command.index = 0;
                break;
            case 'm':
                if(args->command.index != -1) {  
                    fprintf(stderr, "cannot use -n with -m\n");
                    exit(1);
                }
                args->command.index = 1;
                break;
            case 'd':
                if (args->command.index != -1)
                {
                    fprintf(stderr, "cannot use -d  with -m or -n\n");
                    exit(1);
                }
                args->command.index = 2;
                strcpy(args->command.arg, optarg);
                break;
            case 'l':
                if (args->command.index != -1)
                    {
                        fprintf(stderr, "cannot use -l  with -m or -n or -d\n");
                        exit(1);
                    }
                args->command.index = 3;
                break;
            default:
                fprintf(stderr, "unknown argument %d.\n", c);
                exit(1);
        }

    }

    if (optind < argc) {
        fprintf(stderr, "argument not accepted: ");
        while (optind < argc) {
            fprintf(stderr, "%s ", argv[optind++]);
        }
        fprintf(stderr, "\n");
        exit(1);
    }

    if(!nusers){
        fprintf(stderr, "mandatory: -u <user:password>\n");
        exit(1);
    }
    if(!mandatory_address){
        fprintf(stderr, "mandatory: -a <address>\n");
        exit(1);
    }
    if(!mandatory_port){
        fprintf(stderr, "mandatory: -p <port>\n");
        exit(1);
    }
    if(args->command.index == -1){
        fprintf(stderr, "mandatory: -n or -m or -d\n");
        exit(1);
    }
}
