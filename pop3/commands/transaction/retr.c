
#include "../command_service.h"
#include "../../states/write_buffer_helpers.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <fcntl.h>

#define OK_RETR "+OK message follows\r\n"
#define ERRORS_RETR "-ERR no such mesage\r\n"
#define FINISH_RETR "\r\n.\r\n"
int write_in_fd(struct selector_key *key);

typedef struct email_data {
    int connection_fd;
    int email_fd;
    // Esto mismo se utilizara para el byteStuffing  el valor 0 es un caracter cualquiera
    // el valor 1 es el caracter \r y el valor 2 es el caracter \n
    int character_flag;
    bool * isAllDone;
    uint8_t email_buffer[BUFFER_SIZE];
    buffer * connection_buffer;
    buffer  write_buffer;
}email_data;

void pop3_read_email_handler(struct selector_key *key){
    email_data * data = (email_data *) key->data;
    buffer * connection_buffer = data->connection_buffer;
    buffer * write_buffer = &data->write_buffer;
    size_t capacity;
    ssize_t sent;
    uint8_t * readPtr = buffer_write_ptr(write_buffer, &(capacity));

    sent = read(data->email_fd, readPtr, capacity);

    if (sent <= 0 && !buffer_can_read(write_buffer)) {
        *(data->isAllDone) = true;
        // Me tengo que desregristrar
        selector_set_interest_key(key, OP_NOOP);
        selector_set_interest(key->s,data->connection_fd, OP_WRITE);
        return;
    } else {
        buffer_write_adv(write_buffer, sent);
    }
    if (!buffer_can_read(write_buffer))
    {
        printf("Cannot read buffer\n");
    }
    if(!buffer_can_write(connection_buffer)){
        printf("Cannot write buffer\n");
    }

    while (buffer_can_read(write_buffer) && buffer_can_write(connection_buffer)) {
        uint8_t *readPtr = buffer_read_ptr(write_buffer, &(capacity));
        if (*(readPtr) == EOF) {
            *(data->isAllDone) = true;
            break;
        }
        switch (data->character_flag){
            case 0:
                if(*readPtr == '\r')
                    data->character_flag = 1;
                break;
            case 1:
                if(*readPtr == '\n')
                    data->character_flag = 2;
                else
                    data->character_flag = 0;
                break;
            case 2:
                if(*readPtr == '.')
                    data->character_flag = 3;
                else
                    data->character_flag = 0;
                break;

        }
        if(data->character_flag == 3){
            buffer_write(connection_buffer, '.');
            data->character_flag = 0;
        }else
        {
            buffer_write(connection_buffer, *readPtr);
            buffer_read_adv(write_buffer, 1);
        }
    }

    selector_set_interest_key(key, OP_NOOP);
    selector_set_interest(key->s,data->connection_fd, OP_WRITE);
}
void pop3_close_email_handler(struct selector_key *key){
    close(key->fd);
    email_data * data = (email_data *) key->data;
    free(data);
}
void pop3_close_block_handler(struct selector_key *key){

}
static fd_handler handler = {
    .handle_read = pop3_read_email_handler,
    .handle_write = NULL,
    .handle_close = pop3_close_email_handler,
    .handle_block = pop3_close_block_handler,
};

enum pop3_states handle_retr(struct commands_state * ctx, struct selector_key * key) {

    ctx->pop3_current_command->cmd_id = RETR;
    ctx->pop3_current_command->is_finished = false;
    ctx->pop3_current_command->has_error = false;
    ctx->pop3_current_command->retr_state.title_sent = false;
    ctx->pop3_current_command->retr_state.mail_finished = false;

    if (atoi(ctx->arg1) - 1 >= ctx->inbox_data.email_files_length || atoi(ctx->arg1) - 1 < 0) {
        ctx->pop3_current_command->has_error = true;
        return TRANSACTION_STATE;
    }
    if (ctx->inbox_data.email_files[atoi(ctx->arg1) - 1].is_deleted) {
        ctx->pop3_current_command->has_error = true;
        return TRANSACTION_STATE;
    }

    int fd = open(ctx->inbox_data.email_files[atoi(ctx->arg1) - 1].path, O_RDONLY);
    if(fd == -1){
        ctx->pop3_current_command->has_error = true;
        return TRANSACTION_STATE;
    }

    email_data * data = calloc(1,sizeof(email_data));
    data->connection_buffer = &ctx->write_buffer;
    buffer_init(&data->write_buffer,BUFFER_SIZE, data->email_buffer);
    data->email_fd = fd;
    data->connection_fd = key->fd;
    data->character_flag = 2;
    ctx->pop3_current_command->retr_state.mail_fd = fd;
    data->isAllDone = &ctx->pop3_current_command->retr_state.mail_finished;
    selector_register(key->s,fd, &handler, OP_NOOP, data);
    return TRANSACTION_STATE;
}


enum pop3_states handle_write_retr(struct selector_key *key, pop3_current_command *current_command, struct commands_state *commands) {
    if (current_command->has_error) {
        bool has_place = enters_the_buffer(key, ERRORS_RETR);
        if (has_place) {
            long offset = write_in_buffer(key, ERRORS_RETR, strlen(ERRORS_RETR), 0);
            if (offset == -1) {
                current_command->is_finished = true;
            }
        }
    } else {
        if (!current_command->retr_state.title_sent) {
            bool has_place = enters_the_buffer(key, OK_RETR);
            if (has_place) {
                long offset = write_in_buffer(key, OK_RETR, strlen(OK_RETR), 0);
                if (offset == -1) {
                    current_command->retr_state.title_sent = true;
                }
            }
        }
        if (current_command->retr_state.mail_finished) {
            bool has_place = enters_the_buffer(key, FINISH_RETR);
            if (has_place) {
                long offset = write_in_buffer(key, FINISH_RETR, strlen(FINISH_RETR), 0);
                if (offset == -1) {
                    current_command->is_finished = true;
                }
            }
        }

    }
    if(write_in_fd(key)) {
        return TRANSACTION_STATE;
    }else{
        return FORCED_QUIT_STATE;
    }
}
