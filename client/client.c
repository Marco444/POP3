#include "../lib/client//args.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
typedef void (*handler)(int socket, char* buffer, int size,char * args);

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
void metrics_handler(int socket, char* buffer, int size,char * args){
    char* metrics = "METRICS\r\n";
    send(socket, metrics, strlen(metrics), 0);
    flus_socket(socket, buffer, size);
    printf("%s", buffer);
}
void add_user_hanlder(int socket,char * buffer,int size,char * args){
    char add_user[255];
    printf("%s", args);
    sprintf(add_user,"ADD_USER %s\r\n",args);
    send(socket, add_user, strlen(add_user), 0);
    flus_socket(socket, buffer, size);
    printf("%s", buffer);
}
int main(int argc, char** argv) {
    struct client_args args;
    parse_args(argc, argv, &args);
    handler handler []= {add_user_hanlder,metrics_handler};
    int status, valread, client_fd;
    struct sockaddr_in serv_addr;

    char buffer[1024] = { 0 };
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("%s", "\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(args.server_data.server_port);

    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET, args.server_data.server_addr, &serv_addr.sin_addr)
        <= 0) {
        printf("%s", 
                "\nInvalid address/ Address not supported \n");
        return -1;
    }

    if ((status
                 = connect(client_fd, (struct sockaddr*)&serv_addr,
                           sizeof(serv_addr)))
        < 0) {
        printf("%s", "\nConnection Failed \n");
        return -1;
    }

    char  username [255];
    char  password [255];
    sprintf(username, "USERNAME %s\r\n", args.user.name);
    printf("%s", username);
    send(client_fd, username, strlen(username), 0);
    flus_socket(client_fd, buffer, 1024);
    if(buffer[0] != '+') {
        printf("Error: %s\n", buffer);
        return 1;
    }
    sprintf(password,"PASSWORD %s\r\n", args.user.pass);
    send(client_fd, password, strlen(password), 0);
    flush_buffer(buffer, 1024);
    flus_socket(client_fd, buffer, 1024);
    printf("%s", buffer);
    if(buffer[0] != '+') {
        printf("Error: %s\n", buffer);
        return 1;
    }

    handler[args.command.index](client_fd,buffer,1024,args.command.arg);

}
