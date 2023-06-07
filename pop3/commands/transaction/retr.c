
#include "../command_service.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <fcntl.h>
typedef struct email_data {
    int offset;
    int connection_fd;
    int email_fd;
    char email_buffer[BUFFER_SIZE];
    buffer * connection_buffer;
    buffer * write_buffer;
}email_data;
void pop3_read_email_handler(struct selector_key *key){
    email_data * data = (email_data *) key->data;
    buffer * connection_buffer = data->connection_buffer;
    buffer * write_buffer = data->write_buffer;
    size_t capacity;
    ssize_t sent;
    if (!buffer_can_read(write_buffer)) {
        return;
    }
    uint8_t * readPtr = buffer_read_ptr(write_buffer, &(capacity));
    sent = read(data->email_fd, readPtr, capacity);
    if (sent <= 0) {
        // close conection
    } else {
        buffer_read_adv(write_buffer, sent);
    }
    if(buffer_can_read(write_buffer))
        selector_set_interest_key(key, OP_WRITE);
    else
        selector_set_interest_key(key, OP_READ);
    while (buffer_can_read(write_buffer) && buffer_can_write(connection_buffer))
    {
        uint8_t * readPtr = buffer_read_ptr(write_buffer, &(capacity));
        if(*readPtr == EOF){
            close(data->email_fd);
            break;
        }
        buffer_write(connection_buffer, *readPtr);
        buffer_read_adv(write_buffer, 1);
    }
    selector_set_interest_key(key, OP_NOOP);
    selector_set_interest(key->s,data->connection_fd, OP_WRITE);
}
static fd_handler handler = {
    .handle_read = pop3_read_email_handler,
    .handle_write = NULL,
    .handle_close = NULL,
    .handle_block = NULL,
};
enum pop3_states handle_retr(struct commands_state * ctx, struct selector_key *key) {
    printf("RETR %s\n",ctx->email_files[atoi(ctx->arg1) - 1].path);
    int fd = open(ctx->email_files[atoi(ctx->arg1) - 1].path, O_RDONLY);
    if(fd == -1){
        printf("Error opening file\n");
        return TRANSACTION_STATE;
    }
    email_data * data = calloc(1,sizeof(email_data));
    data->connection_buffer = &ctx->write_buffer;
    buffer_init(data->write_buffer,BUFFER_SIZE, data->email_buffer);
    data->email_fd = fd;
    data->connection_fd = key->fd;
    selector_register(key->s,fd, &handler, OP_READ, data);
    elem_type elem = calloc(1,sizeof(struct cmd));
    elem->cmd_id = RETR;
    ctx->email_fd = fd;
    elem->offset = 0;
    elem->is_done = false;
    ctx->write_data = elem;
    return TRANSACTION_STATE; 
}
