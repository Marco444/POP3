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


// progname -> Should be argv[0] program name
static void
usage(const char *progname) {
    fprintf(stderr,
            "Usage: %s [OPTION]...\n"
            "\n"
            "   -h               Imprime la ayuda y termina.\n"
            "   -l <POP3 addr>  Dirección donde servirá el POP3.\n"
            "   -L <conf  addr>  Dirección donde servirá el servicio de monitoreo.\n"
            "   -p <POP3 port>  Puerto entrante conexiones POP3.\n"
            "   -P <conf port>   Puerto entrante conexiones configuracion\n"
            "   -u <name>:<pass> Usuario y contraseña de usuario que puede usar el servidor. Hasta 10.\n"
            "   -v               Imprime información sobre la versión versión y termina.\n"
            "\n"
            "   --doh-ip    <ip>    \n"
            "   --doh-port  <port>  XXX\n"
            "   --doh-host  <host>  XXX\n"
            "   --doh-path  <host>  XXX\n"
            "   --doh-query <host>  XXX\n"

            "\n",
            progname);
    exit(1);
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
                // { "doh-ip",    required_argument, 0, 0xD001 },
                // { "doh-port",  required_argument, 0, 0xD002 },
                // { "doh-host",  required_argument, 0, 0xD003 },
                // { "doh-path",  required_argument, 0, 0xD004 },
                // { "doh-query", required_argument, 0, 0xD005 },
                // { 0,           0,                 0, 0 }
                { "doh-ip",    no_argument, 0, 0xD001 },
                { "doh-port",  no_argument, 0, 0xD002 },
                { "doh-user",  no_argument, 0, 0xD003 },
                { "doh-command",  no_argument, 0, 0xD004 },
                { "doh-newuser",  no_argument, 0, 0xD005 },
                { "doh-metrics",  no_argument, 0, 0xD006 },
                { 0,           0,                 0, 0 }
        };
        c = getopt_long(argc, argv, "a:p:u:n:m", long_options, &option_index);

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

                args->command.index = 0;
                break;
            case 'm':
                if(args->command.index != -1) {
                    fprintf(stderr, "cannot use -n with -m\n");
                    exit(1);
                }
                args->command.index = 1;
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
        fprintf(stderr, "mandatory: -n or -m\n");
        exit(1);
    }
}
