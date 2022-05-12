#include "log.h"
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

static bool quiet_mode = false;

static const char *log_level_strings[] = { "Debug", "Info", "Warn", "Error" };

static const char *log_level_colors[] = {
  "\x1b[36m", "\x1b[33m", "\x1b[35m", "\x1b[31m"
};

static void get_time(char buffer[26])
{
  time_t timer;
  struct tm* tm_info;

  timer = time(NULL);
  tm_info = localtime(&timer);

  strftime(buffer, 26, "[%H:%M:%S]", tm_info);
}

void set_quiet_mode(bool value)
{
  quiet_mode = value;
}

void log_ln(enum log_type type, const char *fmt, ...)
{
  if (quiet_mode && type != LOG_ERROR) {
    return;
  }

  char time[26];
  get_time(time);

  va_list args;
  fprintf(stdout, "\x1b[33m%s \x1b[0m%s%s:\x1b[0m ",
      time, log_level_colors[type], log_level_strings[type]);
  va_start(args, fmt);
  vfprintf(stdout, fmt, args);
  va_end(args);
  fprintf(stdout, "\n");
}
