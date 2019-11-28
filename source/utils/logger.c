#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#include "utils/logger.h"

static const char *level_names[] = {
    "TRACE",
    "DEBUG",
    "INFO",
    "ATTENTION",
    "ERREUR",
    "FATAL",
};

static const char *level_colors[] = {
    "\e[94m",
    "\e[36m",
    "\e[32m",
    "\e[33m",
    "\e[31m",
    "\e[35m",
};

void log_log(int level, const char *file, int line, const char *fmt, ...)
{
    time_t t = time(NULL);
    struct tm *lt = localtime(&t);

    va_list args;
    char buf[16];
    buf[strftime(buf, sizeof(buf), "%H:%M:%S", lt)] = '\0';

    fprintf(
        stderr, "%s %s%-5s\x1b[0m \x1b[90m%s:%d:\x1b[0m ",
        buf, level_colors[level], level_names[level], file, line);

    va_start(args, fmt);
    vfprintf(stderr, fmt, args);
    va_end(args);
    fprintf(stderr, "\n");
    fflush(stderr);
}