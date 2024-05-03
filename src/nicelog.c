#include "nicelog.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static char _app_name[21] = "";

static int _log_level = NL_INFO;
static unsigned int depth = 0;

static const char *log_level_str[] = {"NULL",
                                      "TRACE",
                                      "DEBUG",
                                      "INFO",
                                      (NL_BOLDYELLOW "WARN" NL_RESET),
                                      (NL_BOLDRED "ERROR" NL_RESET),
                                      (NL_BOLDRED "FATAL" NL_RESET),
                                      "NULL"};

static const char *stage_info[] = {(NL_BOLDGREEN "DONE" NL_RESET),
                                   (NL_BOLDYELLOW "SKIPPED" NL_RESET),
                                   (NL_BOLDRED "ABORTED" NL_RESET)};

static unsigned int just_finished_phase = 0;

static unsigned int show_file_and_line = 1;

void NL_set_app_name(char *name) {
  int i = 0;
  int length = strlen(name);
  while (i < 20 && i < length && (_app_name[i] = name[i]) != '\0')
    ++i;
  _app_name[i] = '\0';
}

void NL_reset_app_name() { _app_name[0] = '\0'; }

void NL_set_log_level(unsigned int log_level) { _log_level = log_level; }

void NL_log_output(unsigned int log_level, char *file, unsigned int line,
                   const char *fmt, ...) {
  if (log_level < 1 && log_level > NL_ENUM_LOG_LEVEL_COUNT - 1) {
    WARN("NiceLog: Unknown log level: %d", log_level);
    return;
  }
  if (log_level < _log_level)
    return; // ignore
  va_list args;
  va_start(args, fmt);

  for (int i = 0; i < depth; ++i)
    printf("│  ");

  if (_app_name[0] != '\0')
    printf("[%s]", _app_name);
  printf("[%s] ", log_level_str[log_level]);
  if (show_file_and_line)
    printf("(%s:%d) ", file, line);
  if (log_level == NL_WARN)
    printf(NL_YELLOW);
  else if (log_level > NL_WARN)
    printf(NL_RED);

  vprintf(fmt, args);
  printf(NL_RESET);
  printf("\n");
  just_finished_phase = 0;
}

void NL_phase_begin(char *stage) {
  for (int i = 0; i < depth; ++i)
    printf("│  ");
  if (!just_finished_phase) {
    printf("\n");
    for (int i = 0; i < depth; ++i)
      printf("│  ");
  }
  printf("┌─[" NL_GREEN "%s" NL_RESET "]\n", stage);
  ++depth;
}

void NL_phase_done(unsigned int info) {
  const char *state;
  if (info > NL_ENUM_STAGE_INFO_COUNT) {
    WARN("Unkown stage state");
    state = "DONE";
  } else {
    state = stage_info[info];
  }
  for (int i = 0; i < depth - 1; ++i)
    printf("│  ");
  printf("└" NL_GREEN "%s\n" NL_RESET, state);
  for (int i = 0; i < depth - 1; ++i)
    printf("│  ");
  printf("\n");
  --depth;
  just_finished_phase = 1;
}

void NL_set_file_and_line(int enable) { show_file_and_line = enable; }

void NL_test(void) {
  NL_set_log_level(NL_ALL);
  BEGIN("Initializing app");
  INFO("Hello World! 1+2 = %d", 1 + 2);
  WARN("Unknown Error !");
  BEGIN("Trying something");
  ERROR("Error, skipping");
  DONE(NL_SKIPPED);
  INFO("OK.");
  DONE(NL_OK);
  BEGIN("initializing module 1");
  INFO("initialized");
  DONE(NL_OK);
  BEGIN("initializing module 2");
  INFO("HI");
  FATAL("Unknown error, exiting...");
  DONE(NL_ABORTED);
}
