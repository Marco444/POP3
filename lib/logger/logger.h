#ifndef _LOGGER_H_
#define _LOGGER_H_

// #define DISABLE_LOGGER

#include "../selector/selector.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>

typedef enum {
    LOG_DEBUG = 0,
    LOG_INFO,
    LOG_OUTPUT,
    LOG_WARNING,
    LOG_ERROR,
    LOG_FATAL
} Log_level;

#define MIN_LOG_LEVEL LOG_DEBUG
#define MAX_LOG_LEVEL LOG_FATAL

const char* logger_get_level_string(Log_level level);

#ifdef DISABLE_LOGGER
#define logger_init(selector, log_file, log_stream)
#define logger_finalize()
#define logger_set_level(level)
#define logger_is_enabled_for(level) 0
#define logf(level, format, ...)
#define log(level, s)
#define log_client_authenticated(client_id, username, successful)
#else

int logger_init(fd_selector selector_param, const char* log_file, FILE* log_stream_param);

int logger_finalize();

void logger_set_level(Log_level level);

int logger_is_enabled_for(Log_level level);

void logger_pre_print();

void logger_get_bufstart_and_maxlength(char** bufstart_var, size_t* maxlen_var);

int logger_post_print(int written, size_t maxlen);

//defined as a macro to not make stack overhead
#define logf(level, format, ...)                                                                                                      \
    if (logger_is_enabled_for(level)) {                                                                                                  \
        logger_pre_print();                                                                                                             \
        time_t loginternal_time = time(NULL);                                                                                         \
        struct tm loginternal_tm = *localtime(&loginternal_time);                                                                     \
        size_t loginternal_maxlen;                                                                                                    \
        char* loginternal_bufstart;                                                                                                   \
        logger_get_bufstart_and_maxlength(&loginternal_bufstart, &loginternal_maxlen);                                                    \
        int loginternal_written = snprintf(loginternal_bufstart, loginternal_maxlen, "%04d-%02d-%02d_t%02d:%02d:%02d%s\t" format "\n", \
                                           loginternal_tm.tm_year + 1900, loginternal_tm.tm_mon + 1, loginternal_tm.tm_mday,          \
                                           loginternal_tm.tm_hour, loginternal_tm.tm_min, loginternal_tm.tm_sec,                      \
                                           level == LOG_OUTPUT ? "" : logger_get_level_string(level), ##__VA_ARGS__);                      \
        logger_post_print(loginternal_written, loginternal_maxlen);                                                                     \
    }

#define log(level, s) logf(level, "%s", s)

#endif
#endif
