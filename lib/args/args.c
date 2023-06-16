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
        strcpy(user->name, s);
        strcpy(user->pass, p);
        user->close = false;
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
parse_args(const int argc, char **argv, struct pop3args *args) {
    // Set the struct pop3args to have NULL values, so can be NULL terminated and NULL pointers
    memset(args, 0, sizeof(*args)); // sobre todo para setear en null los punteros de users

    // Default values for POP3 server address and port
    args->pop3_addr = "127.0.0.1";
    args->pop3_port = 8114;

    // Default values for monitor address and port
    args->monitor_addr   = "127.0.0.1";
    args->monitor_port   = 8200;

    char c;
    args->users_count = 0;
    args->users_admin_count = 0;

    // Flags for mandatory parameters
    bool mandatory_mail_dir = false;

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
            { "doh-host",  no_argument, 0, 0xD003 },
            { "doh-path",  no_argument, 0, 0xD004 },
            { "doh-query", no_argument, 0, 0xD005 },
            { 0,           0,                 0, 0 }
        };
        // TODO check if there flags that have to be removed
        c = getopt_long(argc, argv, "hl:L:Np:P:u:vd:a:", long_options, &option_index);
        
        if (c == -1)
            break;
    
        switch (c) {
            case 'h':
                usage(argv[0]);
                break;
            case 'l':
                args->pop3_addr = optarg;
                break;
            case 'L':
                args->monitor_addr = optarg;
                break;
            // case 'N':
            //     args->disectors_enabled = false;
            //     break;
            case 'p':
                args->pop3_port = port(optarg);
                break;
            case 'P':
                args->monitor_port   = port(optarg);
                break;
            case 'u':
                if(args->users_count >= MAX_USERS) {
                    fprintf(stderr, "maximun number of command line users reached: %d.\n", MAX_USERS);
                    exit(1);
                } else {
                    write_user(optarg, args->users + args->users_count);
                    args->users_count += 1;
                }
                break;
            case 'v':
                version();
                exit(0);
                break;
            
            case 'd':
                mandatory_mail_dir = true;
                args->mail_dir = optarg;
                break;

            case 'a':
                if(args->users_admin_count >= MAX_USERS) {
                    fprintf(stderr, "maximun number of admin command line users reached: %d.\n", MAX_USERS);
                    exit(1);
                } else {
                    write_user(optarg, args->users_admin + args->users_admin_count);
                    args->users_admin_count += 1;
                }
                break;
            // TODO check if we have to remove this
            // case 0xD001:
            //     args->doh.ip = optarg;
            //     break;
            // case 0xD002:
            //     args->doh.port = port(optarg);
            //     break;
            // case 0xD003:
            //     args->doh.host = optarg;
            //     break;
            // case 0xD004:
            //     args->doh.path = optarg;
            //     break;
            // case 0xD005:
            //     args->doh.query = optarg;
            //     break;
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

    if(!mandatory_mail_dir){
        fprintf(stderr, "mandatory: -d <maildir>\n");
        exit(1);
    }
}
