#ifndef NICELOG_H_
#define NICELOG_H_

/*
** Macros to use when you want to output a log.
**
** @param fmt,... Use like you would use a printf().
*/
#define TRACE(fmt, ...)                                                        \
  NL_log_output(NL_TRACE, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define DEBUG(fmt, ...)                                                        \
  NL_log_output(NL_DEBUG, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define INFO(fmt, ...)                                                         \
  NL_log_output(NL_INFO, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define WARN(fmt, ...)                                                         \
  NL_log_output(NL_WARN, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define ERROR(fmt, ...)                                                        \
  NL_log_output(NL_ERROR, __FILE__, __LINE__, fmt, ##__VA_ARGS__)
#define FATAL(fmt, ...)                                                        \
  NL_log_output(NL_FATAL, __FILE__, __LINE__, fmt, ##__VA_ARGS__)

/*
** Macros to use when beginning or finishing an important step/action of your
** code. See NL_phase_begin and NL_phase_done below for more info
*/
#define BEGIN(text) NL_phase_begin(text)
#define DONE(info) NL_phase_done(info)

/*
** Used by NiceLog to print Nice logs.
*/
#define NL_RESET "\033[0m"
#define NL_BLACK "\033[30m"              /* Black */
#define NL_RED "\033[31m"                /* Red */
#define NL_GREEN "\033[32m"              /* Green */
#define NL_YELLOW "\033[33m"             /* Yellow */
#define NL_BLUE "\033[34m"               /* Blue */
#define NL_MAGENTA "\033[35m"            /* Magenta */
#define NL_CYAN "\033[36m"               /* Cyan */
#define NL_WHITE "\033[37m"              /* White */
#define NL_BOLDBLACK "\033[1m\033[30m"   /* Bold Black */
#define NL_BOLDRED "\033[1m\033[31m"     /* Bold Red */
#define NL_BOLDGREEN "\033[1m\033[32m"   /* Bold Green */
#define NL_BOLDYELLOW "\033[1m\033[33m"  /* Bold Yellow */
#define NL_BOLDBLUE "\033[1m\033[34m"    /* Bold Blue */
#define NL_BOLDMAGENTA "\033[1m\033[35m" /* Bold Magenta */
#define NL_BOLDCYAN "\033[1m\033[36m"    /* Bold Cyan */
#define NL_BOLDWHITE "\033[1m\033[37m"   /* Bold White */

enum LogLevel {
  NL_ALL = 0,
  NL_TRACE,
  NL_DEBUG,
  NL_INFO,
  NL_WARN,
  NL_ERROR,
  NL_FATAL,
  NL_NONE,

  NL_ENUM_LOG_LEVEL_COUNT
};

enum { NL_DISABLE = 0, NL_ENABLE };

enum StageInfo {
  NL_OK,
  NL_SKIPPED,
  NL_ABORTED,

  NL_ENUM_STAGE_INFO_COUNT
};

/*
** Function to specify your application name to nicelog.
** NiceLog will then print your logs like this:
**    [APP-NAME][INFO] message
**
** @param name max 20 characters
*/
void NL_set_app_name(char *name);

/*
** Function to specify the log level of NiceLog.
** Any output under the log level will be ignored
** Example : if you set the log level to NL_INFO, debug and trace output will be
**           ignored.
**
** @param logLevel Any value of the LogLevel enum
*/
void NL_set_log_level(unsigned int logLevel);

/*
** Function to print out a log in stdout. It's better to use the macros defined
** at the top of this file.
**
** @param logLevel Any value of the LogLevel enum
** @param file The c file in which this function is used (for debugging
**             purposes). Use the C '__FILE__' macro.
** @param line The line at which this function is used (for debugging purposes).
**             Use the C '__LINE__' macro.
** @param fmt Your format text, the same as when using printf().
** @param ... Your additionnal arguments, the sames as when using printf().
*/
void NL_log_output(unsigned int logLevel, char *file, unsigned int line,
                   const char *fmt, ...);

/*
** Function to use when beginning an important step/action of your code
** (initializing, loading something...; can be nested).
**
** @param stage The step/action which will be run.
**              Example: "Initializing App...".
*/
void NL_phase_begin(char *stage);

/*
** Funtion to use when an important step/action is done
** There can be several outcomes (NL_OK, NL_SKIPPED, NL_ABORTED)
**
** @param info The outcome.
*/
void NL_phase_done(unsigned int info);

/*
** Function to enable or disable showing the line and file where the log has
** been printed. Default: enabled.
**
** @param enable Either NL_DISABLE or NL_ENABLE
*/
void NL_set_file_and_line(int enable);

/*
** Example output when using NiceLog:
*/

/*    ┌─[Initializing app]
 *    │  [WARN] Hello World! 1+2 = 3
 *    │  [ERROR] Unknown Error !
 *    │
 *    │  ┌─[Trying something]
 *    │  │  [INFO] OK.
 *    │  └DONE
 *    │
 *    │  [FATAL] Unknown fatal error ! Exiting...
 *    └ABORTED
 */
#endif // NICELOG_H_
