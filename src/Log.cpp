#include "Log.h"

// Initialize the log level mappings
const LogLevelMap Log::logLevels[] = {
    {"NONE",    LOG_NONE   },
    {"ERROR",   LOG_ERROR  },
    {"WARN",    LOG_WARN   },
    {"INFO",    LOG_INFO   },
    {"DEBUG",   LOG_DEBUG  },
    {"VERBOSE", LOG_VERBOSE}
};

// Initialize the size of the log level mappings array
const int Log::logLevelsSize = sizeof(Log::logLevels) / sizeof(LogLevelMap);

// Initialize the global log level setting
int Log::logLevelGlobal = LOG_INFO;

const char *Log::getLogLevelName(LogLevel level) {
    for (int i = 0; i < logLevelsSize; i++) {
        if (logLevels[i].level == level) {
            return logLevels[i].name;
        }
    }
    // Return "UNKNOWN" if the level is not found
    return "UNKNOWN";
}

void Log::printSerialLine(LogLevel level, const char *glyph, const char *tag, const char *logMessage, va_list args) {
    // Check if serial connection is available and only continue if `level` is higher than globally set log level
    if (Serial) {
        // TODO(ksatirli): fix this
        // if (Serial && level <= logLevelGlobal) {
        // Create buffer for log line content
        char logLine[kLogLineBufferSize];

        // Handle variable arguments
        vsnprintf(logLine, sizeof(logLine), logMessage, args);

        // Send assembled log line to serial output
        Serial.printf("[%s][%s %s] %s\n", tag, glyph, getLogLevelName(level), logLine);
    }
}

void Log::debug(const char *logTag, const char *logMessage, ...) {
    // Handle variable arguments:
    va_list args;
    va_start(args, logMessage);

    // Assemble log line:
    printSerialLine(LOG_DEBUG, "🟦", logTag, logMessage, args);

    va_end(args);
}

void Log::error(const char *logTag, const char *logMessage, ...) {
    // Handle variable arguments:
    va_list args;
    va_start(args, logMessage);

    // Assemble log line:
    printSerialLine(LOG_ERROR, "🟥", logTag, logMessage, args);

    va_end(args);
}

void Log::info(const char *logTag, const char *logMessage, ...) {
    // Handle variable arguments:
    va_list args;
    va_start(args, logMessage);

    // Assemble log line:
    printSerialLine(LOG_INFO, "🟩", logTag, logMessage, args);

    va_end(args);
}

void Log::warning(const char *logTag, const char *logMessage, ...) {
    // Handle variable arguments:
    va_list args;
    va_start(args, logMessage);

    // Assemble log line:
    printSerialLine(LOG_WARN, "🟨", logTag, logMessage, args);

    va_end(args);
}

void Log::verbose(const char *logTag, const char *logMessage, ...) {
    // Handle variable arguments:
    va_list args;
    va_start(args, logMessage);

    // Assemble log line:
    printSerialLine(LOG_VERBOSE, "⬛", logTag, logMessage, args);

    va_end(args);
}

void Log::setLevel(LogLevel level) {
    const char *logTag = __func__;

    info(logTag, "Setting log level to %d", level);

    // Update global log level
    logLevelGlobal = level;
}
