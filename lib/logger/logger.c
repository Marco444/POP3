#include "logger.h"

void log_(FILE * const stream, const char * color, const char * prefix, const char * const format, const char * suffix, va_list arguments) {
	time_t currentTime;
    struct tm *localTime;

    currentTime = time(NULL);

    localTime = localtime(&currentTime);

	fprintf(stream, "%s%s %04d-%02d-%02d  %02d:%02d:%02d ", color, prefix,localTime->tm_year + 1900,localTime->tm_mon + 1,localTime->tm_mday,localTime->tm_hour,localTime->tm_min,localTime->tm_sec);
	vfprintf(stream, format, arguments);
	fprintf(stream, "%s%s", suffix, ANSI_COLOR_RESET);
}

void log_debug(const char * const format, ...) {
	#ifdef DEBUG
	va_list arguments;
	va_start(arguments, format);
	log_(stdout, ANSI_COLOR_BLUE, "[DEBUG] ", format, "\n", arguments);
	va_end(arguments);
	#endif
}

void log_error(const char * const format, ...) {
	va_list arguments;
	va_start(arguments, format);
	log_(stderr, ANSI_COLOR_RED, "[ERROR] ", format, "\n", arguments);
	va_end(arguments);
}

void log_error_raw(const char * const format, ...) {
	va_list arguments;
	va_start(arguments, format);
	log_(stderr, ANSI_COLOR_RED, "", format, "", arguments);
	va_end(arguments);
}

void log_info(const char * const format, ...) {
	#ifdef LOGGING
	va_list arguments;
	va_start(arguments, format);
	log_(stdout, ANSI_COLOR_GREEN, "[INFO] ", format, "\n", arguments);
	va_end(arguments);
    #endif
}
