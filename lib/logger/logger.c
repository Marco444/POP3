#include "logger.h"
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#define DEFAULT_LOG_FOLDER "./log"
#define DEFAULT_LOG_FILE (DEFAULT_LOG_FOLDER "/%02d-%02d-%04d.log")
#define DEFAULT_LOG_FILE_MAXSTRLEN 48

#define LOG_MIN_BUFFER_SIZE 0x1000
#define LOG_MAX_BUFFER_SIZE 0x400000 
#define LOG_BUFFER_SIZE_GRANULARITY 0x1000 
#define LOG_BUFFER_MAX_PRINT_LENGTH 0x200

#define LOG_FILE_PERMISSION_BITS 666
#define LOG_FOLDER_PERMISSION_BITS 666
#define LOG_FILE_OPEN_FLAGS (O_WRONLY | O_APPEND | O_CREAT | O_NONBLOCK)

const char* logger_get_level_string(Log_level level) {
    switch (level) {
        case LOG_DEBUG:
            return " [DEBUG]";
        case LOG_INFO:
            return " [INFO]";
        case LOG_OUTPUT:
            return " [OUTPUT]";
        case LOG_WARNING:
            return " [WARNING]";
        case LOG_ERROR:
            return " [ERROR]";
        case LOG_FATAL:
            return " [FATAL]";
        default:
            return " [UNKNOWN]";
    }
}

#ifndef DISABLE_LOGGER

/** The buffer where logs are buffered. */
static char* buffer = NULL;
static size_t buffer_start = 0, buffer_length = 0, buffer_capacity = 0;

/** The file descriptor for writing logs to disk, or -1 if we're not doing that. */
static int log_file_fd = -1;
static fd_selector selector = NULL;
static Log_level log_level = MIN_LOG_LEVEL;

/** The stream for writing logs to, or NULL if we're not doing that. */
static FILE* log_stream = NULL;

static inline void make_buffer_space(size_t len) {
    if (buffer_length + buffer_start + len > buffer_capacity) {
        if (buffer_capacity <= len) {
            memmove(buffer, buffer + buffer_start, buffer_length);
            buffer_start = 0;
        } else if (buffer_capacity < LOG_MAX_BUFFER_SIZE) {
            size_t new_buffer_capacity = buffer_length + len;
            new_buffer_capacity = (new_buffer_capacity + LOG_BUFFER_SIZE_GRANULARITY - 1) / LOG_BUFFER_SIZE_GRANULARITY * LOG_BUFFER_SIZE_GRANULARITY;
            if (new_buffer_capacity > LOG_MAX_BUFFER_SIZE)
                new_buffer_capacity = LOG_MAX_BUFFER_SIZE;

            void* new_buffer = malloc(new_buffer_capacity);
            if (new_buffer == NULL) {
                memmove(buffer, buffer + buffer_start, buffer_length);
                buffer_start = 0;
            } else {
                memcpy(new_buffer, buffer + buffer_start, buffer_length);
                free(buffer);
                buffer = new_buffer;
                buffer_capacity = new_buffer_capacity;
                buffer_start = 0;
            }
        }
    }
}

static inline void try_flush_buffer_to_file() {
    ssize_t written = write(log_file_fd, buffer + buffer_start, buffer_length);
    if (written > 0) {
        buffer_length -= written;
        buffer_start = (buffer_length == 0 ? 0 : (buffer_start + written));
    }

    selector_set_interest(selector, log_file_fd, buffer_length > 0 ? OP_WRITE : OP_NOOP);
}

static void fd_write_handler(struct selector_key * key) {
    try_flush_buffer_to_file();
}

static void fd_close_handler(struct selector_key* key) {

    if (buffer_length != 0) {
        int flags = fcntl(log_file_fd, F_GETFD, 0);
        fcntl(log_file_fd, F_SETFL, flags & (~O_NONBLOCK));
        ssize_t written = write(log_file_fd, buffer, buffer_length);
        if (written > 0) {
            buffer_length -= written;
            buffer_start = (buffer_length == 0 ? 0 : (buffer_start + written));
        }
    }

    close(log_file_fd);
    log_file_fd = -1;
}

static fd_handler log_fd_handler = {
    .handle_read = NULL,
    .handle_write = fd_write_handler,
    .handle_close = fd_close_handler,
    .handle_block = NULL};

static int try_open_logfile(const char* log_file, struct tm tm) {
    if (log_file == NULL)
        return -1;

    char logfilebuf[DEFAULT_LOG_FILE_MAXSTRLEN + 1];

    if (log_file[0] == '\0') {
        snprintf(logfilebuf, DEFAULT_LOG_FILE_MAXSTRLEN, DEFAULT_LOG_FILE, tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
        log_file = logfilebuf;

        mkdir(DEFAULT_LOG_FOLDER, LOG_FOLDER_PERMISSION_BITS);
    }

    int fd = open(log_file, LOG_FILE_OPEN_FLAGS, LOG_FILE_PERMISSION_BITS);
    if (fd < 0) {
        fprintf(stderr, "WARNING: Failed to open logging file at %s. The server will still run, but with logging disabled.\n", log_file);
        return -1;
    }

    return fd;
}

int logger_init(fd_selector selector_param, const char* log_file, FILE* log_stream_param) {
    time_t time_now = time(NULL);
    struct tm tm = *localtime(&time_now);

    selector = selector_param;
    log_file_fd = selector_param == NULL ? -1 : try_open_logfile(log_file, tm);
    log_stream = log_stream_param;
    log_level = MIN_LOG_LEVEL;

    if (log_file_fd >= 0)
        selector_register(selector, log_file_fd, &log_fd_handler, OP_NOOP, NULL);

    if (log_file_fd >= 0 || log_stream != NULL) {
        buffer = malloc(LOG_MIN_BUFFER_SIZE);
        buffer_capacity = LOG_MIN_BUFFER_SIZE;
        buffer_length = 0;
        buffer_start = 0;
        if (buffer == NULL) {
            close(log_file_fd);
            log_file_fd = -1;
            fprintf(stderr, "WARNING: Failed to malloc a buffer for logging. You don't have 4KBs?\n");
            return -1;
        }
    }

    return 0;
}

int logger_finalize() {
    if (log_file_fd >= 0) {
        selector = NULL;
    }

    if (buffer != NULL) {
        free(buffer);
        buffer = NULL;
        buffer_capacity = 0;
        buffer_length = 0;
        buffer_start = 0;
    }

    log_stream = NULL;
    return 0;
}

void logger_set_level(Log_level level) {
    log_level = level;
}

int logger_is_enabled_for(Log_level level) {
    return level >= log_level && (log_file_fd > 0 || log_stream != NULL);
}

void logger_pre_print() {
    make_buffer_space(LOG_BUFFER_MAX_PRINT_LENGTH);
}

void logger_get_bufstart_and_maxlength(char** bufstart_var, size_t* maxlen_var) {
    *maxlen_var = buffer_capacity - buffer_length - buffer_start;
    *bufstart_var = buffer + buffer_start + buffer_length;
}

int logger_post_print(int written, size_t maxlen) {
    if (written < 0) {
        fprintf(stderr, "Error: snprintf(): %s\n", strerror(errno));
        return -1;
    }

    if ((size_t)written >= maxlen) {
        fprintf(stderr, "Error: %lu bytes of logs possibly lost. Slow disk?\n", written - maxlen + 1);
        written = maxlen - 1;
    }

    if (log_stream != NULL)
        fprintf(log_stream, "%s", buffer + buffer_start + buffer_length);

    if (log_file_fd >= 0) {
        buffer_length += written;
        try_flush_buffer_to_file();
    }
    return 0;
}
#endif
