#ifndef LOG_H_
#define LOG_H_

#include <Arduino.h>

#include <cstdarg>
#include <cstddef>

////////////////////////////////////////////////////////////////////////////////
// CONFIGURATION                                                              //
////////////////////////////////////////////////////////////////////////////////

extern const size_t kLogLineBufferSize;

typedef enum { LOG_NONE = 0, LOG_ERROR = 1, LOG_WARN = 2, LOG_INFO = 3, LOG_DEBUG = 4, LOG_VERBOSE = 5 } LogLevel;

struct LogLevelMap {
    const char *name;
    LogLevel    level;
};

extern int logLevelGlobal;

extern const LogLevelMap logLevels[];

extern const int logLevelsSize;
////////////////////////////////////////////////////////////////////////////////

/**
 * @brief Converts a numeric log level to its corresponding string representation.
 *
 * @param level The numeric log level.
 *
 * @return The string representation of the log level.
 */
const char *getLogLevelName(LogLevel level);

/**
 * @brief Prints a log message to the serial output.
 *
 * @param level The level of the log message.
 * @param glyph The emoji representing the log level.
 * @param tag The tag identifying the log source.
 * @param logMessage The log message format string.
 * @param ... Additional arguments to format the log message.
 */
void logPrintSerialLine(LogLevel level, const char *glyph, const char *tag, const char *logMessage, ...);

/**
 * @brief Logs a message at `DEBUG` level.
 *
 * @param logTag The tag identifying the log source.
 * @param logMessage The log message format string.
 * @param ... Additional arguments to format the log message.
 */
void logDebug(const char *logTag, const char *logMessage, ...);

/**
 * @brief Logs a message at `ERROR` level.
 *
 * @param logTag The tag identifying the log source.
 * @param logMessage The log message format string.
 * @param ... Additional arguments to format the log message.
 */
void logError(const char *logTag, const char *logMessage, ...);

/**
 * @brief Logs a message at `INFO` level.
 *
 * @param logTag The tag identifying the log source.
 * @param logMessage The log message format string.
 * @param ... Additional arguments to format the log message.
 */
void logInfo(const char *logTag, const char *logMessage, ...);

/**
 * @brief Logs a message at `WARN` level.
 *
 * @param logTag The tag identifying the log source.
 * @param logMessage The log message format string.
 * @param ... Additional arguments to format the log message.
 */
void logWarning(const char *logTag, const char *logMessage, ...);

/**
 * @brief Logs a message at `VERBOSE` level.
 *
 * @param logTag The tag identifying the log source.
 * @param logMessage The log message format string.
 * @param ... Additional arguments to format the log message.
 */
void logVerbose(const char *logTag, const char *logMessage, ...);

/**
 * @brief Updates the global log level setting.
 *
 * @param level The new log level.
 */
void logSetLevel(LogLevel level);

#endif  // LOG_H_
