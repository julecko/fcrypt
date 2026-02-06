#ifndef LOGGER_H
#define LOGGER_H

#include <stddef.h>

typedef enum {
    LOGGING_NORMAL,
    LOGGING_VERBOSE
} LoggerLevel;

void logger_init(LoggerLevel level);
void logger_log(const char *msg, ...);

#endif
