#include "clientLibs/args.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main(int argc, char** argv) {
    struct client_args args;
    parse_args(argc, argv, &args);

    int status, valread, client_fd;
    struct sockaddr_in serv_addr;

    char buffer[1024] = { 0 };
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(args.server_data.server_port);

    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET, args.server_data.server_addr, &serv_addr.sin_addr)
        <= 0) {
        printf(
                "\nInvalid address/ Address not supported \n");
        return -1;
    }

    if ((status
                 = connect(client_fd, (struct sockaddr*)&serv_addr,
                           sizeof(serv_addr)))
        < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    char  username [255];
    char  password [255];
    sprintf(username, "USERNAME %s\r\n", args.user.name);

    send(client_fd, username, strlen(username), 0);
    recv(client_fd, buffer, strlen(username), 0);
    if(buffer[0] != '+') {
        printf("Error: %s\n", buffer);
        return -1;
    }
    sprintf(password,"PASSWORD %s\r\n", args.user.pass);
    send(client_fd, password, strlen(password), 0);
    recv(client_fd, buffer, strlen(password), 0);
    if(buffer[0] != '+') {
        printf("Error: %s\n", buffer);
        return -1;
    }
    printf("%s\n", buffer);
}
