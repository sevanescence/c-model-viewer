#ifndef LOGGING_H
#define LOGGING_H

#include <stdio.h>

#ifdef DEBUG
#define mklog(message) { printf(message); fflush(stdout); }
#define mklog_fmt(message, ...) { printf(message, __VA_ARGS__); fflush(stdout); }
#else
#define mklog(message)
#define mklog_fmt(message, ...)
#endif

#endif