#ifndef LOGGING_H
#define LOGGING_H

#include <stdio.h>

#define mklog(message) {printf(message); fflush(stdout);}
#define mklog_fmt(message, ...) { printf(message"\n", __VA_ARGS__); fflush(stdout); }

#endif