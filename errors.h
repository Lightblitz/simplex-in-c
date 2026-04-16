#ifndef ERRORS_H
#define ERRORS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

/* ---------- NULL pointer check macro ---------- */
#define NP_CHECK(ptr) \
    if (!(ptr)) { \
        fprintf(stderr, "%s:%d NULL POINTER: %s\n", \
            __FILE__, __LINE__, (#ptr)); \
        exit(EXIT_FAILURE); \
    }

/* ---------- Debug enable toggle ---------- */
#define DEBUG_TRUE 1  /* set to 0 to disable debug logs */

/* ---------- ANSI color codes ---------- */
#define BLACK  "\033[0;30m"
#define RED    "\033[0;31m"
#define GREEN  "\033[0;32m"
#define YELLOW "\033[0;33m"
#define BLUE   "\033[0;34m"
#define PURPLE "\033[0;35m"
#define CYAN   "\033[0;36m"
#define WHITE  "\033[0;37m"
#define RESET  "\033[0m"

/* ---------- Logging function prototypes ---------- */
void mat_log(FILE *stream, const char *file_name,
             unsigned int line, const char *format, ...);

void mat_vlog(FILE *stream, const char *file_name,
              unsigned int line, const char *format, va_list argp);

/* ---------- Logging macros ---------- */
#define MAT_FLOG(stream, fmt, ...) \
    if (DEBUG_TRUE) { \
        mat_log(stream, __FILE__, __LINE__, fmt, __VA_ARGS__); \
    }

#define MAT_FINFO(fmt, ...) \
    if (DEBUG_TRUE) { \
        mat_log(stdout, __FILE__, __LINE__, fmt, __VA_ARGS__); \
    }

#define MAT_FERROR(fmt, ...) \
    if (DEBUG_TRUE) { \
        mat_log(stderr, __FILE__, __LINE__, RED fmt RESET, __VA_ARGS__); \
    }

#define MAT_ERROR(fmt) \
    if (DEBUG_TRUE) { \
        mat_log(stderr, __FILE__, __LINE__, RED fmt RESET); \
    }

#endif /* ERRORS_H */
