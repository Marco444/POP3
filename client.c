#include "clientLibs/args.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

void flush_buffer(char* buffer, int size) {
    for (int i = 0; i < size; i++) {
        buffer[i] = 0;
    }
}
void flus_socket(int socket, char* buffer,int size){
    int count = 0;
    int flag = true;
    int position = 0;
    while (flag){
        int rec = recv(socket, (buffer + count), 1024, 0);
        for (int i = count;i < (count + rec); ++i) {
            switch (buffer[i]) {
                case '\r':
                    position = 1;
                    break;
                case '\n':
                    if (position == 1) {
                        flag = false;
                    }
                    break;
                default:
                    position = 0;
                    break;
            }
        }
        count += rec;
    }
}


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
    printf(username);
    send(client_fd, username, strlen(username), 0);
    flus_socket(client_fd, buffer, 1024);
    if(buffer[0] != '+') {
        printf("LLEGUE Al username\n", buffer);
        printf("Error: %s\n", buffer);
        return 1;
    }
    sprintf(password,"PASSWORD %s\r\n", args.user.pass);
    send(client_fd, password, strlen(password), 0);
    flush_buffer(buffer, 1024);
    flus_socket(client_fd, buffer, 1024);
    printf(buffer);
    if(buffer[0] != '+') {
        printf("Error: %s\n", buffer);
        return 1;
    }



}
