#ifndef LOGGER_H
#define LOGGER_H

typedef enum
{
    LOG_LEVEL_TRACE,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_FATAL
} log_level_t;

extern void loggerInitialize(void);
extern void log(log_level_t log_level, const char* fmt, ...);

#define PH_LOG_TRACE(...) log(LOG_LEVEL_TRACE, __VA_ARGS__)
#define PH_LOG_INFO(...) log(LOG_LEVEL_INFO, __VA_ARGS__)
#define PH_LOG_WARN(...) log(LOG_LEVEL_WARN, __VA_ARGS__)
#define PH_LOG_ERROR(...) log(LOG_LEVEL_ERROR, __VA_ARGS__)
#define PH_LOG_FATAL(...) log(LOG_LEVEL_FATAL, __VA_ARGS__)

#endif // LOGGER_H