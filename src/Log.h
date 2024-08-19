#ifndef LOG_H_
#define LOG_H_

#include <Arduino.h>

/**
 * @brief Log levels enumeration.
 */
typedef enum { LOG_NONE = 0, LOG_ERROR = 1, LOG_WARN = 2, LOG_INFO = 3, LOG_DEBUG = 4, LOG_VERBOSE = 5 } LogLevel;

/**
 * @brief Mapping of log level names to their corresponding log level values.
 */
struct LogLevelMap {
    const char *name;
    LogLevel    level;
};

/**
 * @brief Log class for managing and printing log messages.
 */
class Log {
 private:
    // Configuration
    constexpr static size_t kLogLineBufferSize = 128;

    // Global log level setting
    static int logLevelGlobal;

    // Log levels and their corresponding integer values
    static const LogLevelMap logLevels[];

    // Size of the log level mappings array
    static const int logLevelsSize;

    /**
     * @brief Prints a log message to the serial output.
     *
     * @param level The level of the log message.
     * @param glyph The emoji representing the log level.
     * @param tag The tag identifying the log source.
     * @param logMessage The log message format string.
     * @param args Additional arguments to format the log message.
     */
    static void printSerialLine(LogLevel level, const char *glyph, const char *tag, const char *logMessage, va_list args);

    /**
     * @brief Converts a numeric log level to its corresponding string representation.
     *
     * @param level The numeric log level.
     *
     * @return The string representation of the log level.
     */
    static const char *getLogLevelName(LogLevel level);

 public:
    /**
     * @brief Logs a message at `DEBUG` level.
     *
     * @param logTag The tag identifying the log source.
     * @param logMessage The log message format string.
     * @param ... Additional arguments to format the log message.
     */
    static void debug(const char *logTag, const char *logMessage, ...);

    /**
     * @brief Logs a message at `ERROR` level.
     *
     * @param logTag The tag identifying the log source.
     * @param logMessage The log message format string.
     * @param ... Additional arguments to format the log message.
     */
    static void error(const char *logTag, const char *logMessage, ...);

    /**
     * @brief Logs a message at `INFO` level.
     *
     * @param logTag The tag identifying the log source.
     * @param logMessage The log message format string.
     * @param ... Additional arguments to format the log message.
     */
    static void info(const char *logTag, const char *logMessage, ...);

    /**
     * @brief Logs a message at `WARN` level.
     *
     * @param logTag The tag identifying the log source.
     * @param logMessage The log message format string.
     * @param ... Additional arguments to format the log message.
     */
    static void warning(const char *logTag, const char *logMessage, ...);

    /**
     * @brief Logs a message at `VERBOSE` level.
     *
     * @param logTag The tag identifying the log source.
     * @param logMessage The log message format string.
     * @param ... Additional arguments to format the log message.
     */
    static void verbose(const char *logTag, const char *logMessage, ...);

    /**
     * @brief Updates the global log level setting.
     *
     * @param level The new log level.
     */
    static void setLevel(LogLevel level);

    /**
     * @brief Gets the log levels array.
     *
     * @return The log levels array.
     */
    static const LogLevelMap *getLogLevels() {
        return logLevels;
    }

    /**
     * @brief Gets the size of the log levels array.
     *
     * @return The size of the log levels array.
     */
    static int getLogLevelsSize() {
        return logLevelsSize;
    }
};

#endif  // LOG_H_
