#include "errors.h"

/* Timestamp helper */
static void get_timestamp(char *buffer, size_t size) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buffer, size, "%Y-%m-%d %H:%M:%S", t);
}

void mat_log(FILE *stream, const char *file_name,
             unsigned int line, const char *format, ...) {
    char timestamp[20];
    get_timestamp(timestamp, sizeof(timestamp));

    fprintf(stream, "[%s] (%s:%u) ", timestamp, file_name, line);

    va_list args;
    va_start(args, format);
    vfprintf(stream, format, args);
    va_end(args);

    fprintf(stream, "\n");
}

void mat_vlog(FILE *stream, const char *file_name,
              unsigned int line, const char *format, va_list argp) {
    char timestamp[20];
    get_timestamp(timestamp, sizeof(timestamp));

    fprintf(stream, "[%s] (%s:%u) ", timestamp, file_name, line);
    vfprintf(stream, format, argp);
    fprintf(stream, "\n");
}
