#pragma once

#include <stdio.h>
#include <stdarg.h>

enum
{
    LOGGER_TRACE,
    LOGGER_DEBUG,
    LOGGER_INFO,
    LOGGER_WARN,
    LOGGER_ERROR,
    LOGGER_FATAL
};

#define log_trace(...) log_log(LOGGER_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define log_debug(...) log_log(LOGGER_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define log_info(...) log_log(LOGGER_INFO, __FILE__, __LINE__, __VA_ARGS__)
#define log_warn(...) log_log(LOGGER_WARN, __FILE__, __LINE__, __VA_ARGS__)
#define log_error(...) log_log(LOGGER_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define log_fatal(...) log_log(LOGGER_FATAL, __FILE__, __LINE__, __VA_ARGS__)

void log_log(int level, const char *file, int line, const char *fmt, ...);