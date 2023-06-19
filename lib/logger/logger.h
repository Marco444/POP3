#ifndef LOGGER_HEADER
#define LOGGER_HEADER

#include <stdarg.h>
#include <stdio.h>
#include <time.h>


// Uncomment to log in DEBUG mode (with more details).
#define DEBUG true
#define LOGGING true

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

void log_(FILE * const stream, const char * color, const char * prefix, const char * const format, const char * suffix, va_list arguments);

void log_debug(const char * const format, ...);

void log_error(const char * const format, ...);

void log_error_raw(const char * const format, ...);

void log_info(const char * const format, ...);

#endif

