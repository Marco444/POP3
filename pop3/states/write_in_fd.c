// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "../../lib/metrics/metrics.h"
#include "../commands/command_service.h"
#include "../../lib/logger/logger.h"
#include "../pop3_states.h"
#include "../monitor_states.h"
#include <stdio.h>
#include <sys/socket.h>

//for mac OS
#ifndef MSG_NOSIGNAL
#define MSG_NOSIGNAL 0
#endif

int write_in_fd(struct selector_key *key){
    int targetFd = key->fd;
    buffer* targetBuffer = &((struct connection_state *) key->data)->commands.write_buffer;


    size_t capacity;
    ssize_t sent;
    if (!buffer_can_read(targetBuffer)) {
       return 1;
    }
    uint8_t * readPtr = buffer_read_ptr(targetBuffer, &(capacity));

    sent = send(targetFd, readPtr, capacity, MSG_NOSIGNAL);

    if (sent <= 0) {
        long_error("Error sending data to client");
        return 0 ;
    } else {
        metrics_register_bytes_transferred(sent);
        buffer_read_adv(targetBuffer, sent);
        return 1;
    }
}

int write_in_fd_monitor(struct selector_key *key){
    int targetFd = key->fd;
    buffer* targetBuffer = &((struct monitor_connection_state *) key->data)->commands.write_buffer;


    size_t capacity;
    ssize_t sent;
    if (!buffer_can_read(targetBuffer)) {
        return 1;
    }
    uint8_t * readPtr = buffer_read_ptr(targetBuffer, &(capacity));
    sent = send(targetFd, readPtr, capacity, MSG_NOSIGNAL);
    if (sent <= 0) {
        return 0 ;
    } else {
        metrics_register_bytes_transferred(sent);
        buffer_read_adv(targetBuffer, sent);
        return 1;
    }
}
