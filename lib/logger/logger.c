#include "logger.h"

void log(FILE * const stream, const char * color, const char * prefix, const char * const format, const char * suffix, va_list arguments) {
	fprintf(stream, "%s%s", color, prefix);
	vfprintf(stream, format, arguments);
	fprintf(stream, "%s%s", suffix, ANSI_COLOR_RESET);
}

void log_debug(const char * const format, ...) {
	#ifdef DEBUG
	va_list arguments;
	va_start(arguments, format);
	log(stdout, ANSI_COLOR_BLUE, "[DEBUG] ", format, "\n", arguments);
	va_end(arguments);
	#endif
}

void log_error(const char * const format, ...) {
	va_list arguments;
	va_start(arguments, format);
	log(stderr, ANSI_COLOR_RED, "[ERROR] ", format, "\n", arguments);
	va_end(arguments);
}

void log_error_raw(const char * const format, ...) {
	va_list arguments;
	va_start(arguments, format);
	log(stderr, ANSI_COLOR_RED, "", format, "", arguments);
	va_end(arguments);
}

void log_info(const char * const format, ...) {
	#ifdef LOGGING
	va_list arguments;
	va_start(arguments, format);
	log(stdout, ANSI_COLOR_GREEN, "[INFO ] ", format, "\n", arguments);
	va_end(arguments);
    #endif
}
