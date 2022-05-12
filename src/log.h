#ifndef LOG_H
#define LOG_H

#include <stdbool.h>

enum log_type {
  LOG_DEBUG = 0,
  LOG_INFO = 1,
  LOG_WARN = 2,
  LOG_ERROR = 3
};

void set_quiet_mode(bool);
void log_ln(enum log_type, const char*, ...);

#define dbgln(...) log_ln(LOG_DEBUG, __VA_ARGS__)
#define infln(...) log_ln(LOG_INFO, __VA_ARGS__)
#define wrnln(...) log_ln(LOG_WARN, __VA_ARGS__)
#define errln(...) log_ln(LOG_ERROR, __VA_ARGS__)

#endif
