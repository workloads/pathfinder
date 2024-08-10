#include "Log.h"

////////////////////////////////////////////////////////////////////////////////
// CONFIGURATION                                                              //
////////////////////////////////////////////////////////////////////////////////

// initialize log line buffer
constexpr size_t kLogLineBufferSize = 128;

// TODO(ksatirli): Set global log level from CORE_DEBUG_LEVEL
// ⚠️ Levels `LOG_INFO`, `LOG_DEBUG`, and `LOG_VERBOSE` may result in a performance decrease
int logLevelGlobal = LOG_ERROR;

// Define log levels and their corresponding integer values
const LogLevelMap logLevels[] = {
    {"NONE",    LOG_NONE   },
    {"ERROR",   LOG_ERROR  },
    {"WARN",    LOG_WARN   },
    {"INFO",    LOG_INFO   },
    {"DEBUG",   LOG_DEBUG  },
    {"VERBOSE", LOG_VERBOSE}
};

const int logLevelsSize = sizeof(logLevels) / sizeof(LogLevelMap);

////////////////////////////////////////////////////////////////////////////////

const char *getLogLevelName(LogLevel level) {
    for (int i = 0; i < logLevelsSize; i++) {
        if (logLevels[i].level == level) {
            return logLevels[i].name;
        }
    }

    // Return "UNKNOWN" if the level is not found
    return "UNKNOWN";
}

void logPrintSerialLine(LogLevel level, const char *glyph, const char *tag, const char *logMessage, va_list args) {
    // Debug print to check log levels
    //    Serial.printf("logLevelGlobal: %d, level: %d\n", logLevelGlobal, level);

    // Check if serial connection is available and only continue if `level` is higher than globally set log level
    if (Serial) {
        // TODO(ksatirli) fix this
        // if (Serial && level <= logLevelGlobal) {
        // Create buffer for log line content
        char logLine[kLogLineBufferSize];

        // Handle variable arguments
        vsnprintf(logLine, sizeof(logLine), logMessage, args);

        // Send assembled log line to serial output
        Serial.printf("[%s][%s %s] %s\n", tag, glyph, getLogLevelName(level), logLine);
    }
}

void logDebug(const char *logTag, const char *logMessage, ...) {
    // Handle variable arguments:
    va_list args;
    va_start(args, logMessage);

    // Assemble log line:
    logPrintSerialLine(LOG_DEBUG, "🟦", logTag, logMessage, args);

    va_end(args);
}

void logError(const char *logTag, const char *logMessage, ...) {
    // Handle variable arguments:
    va_list args;
    va_start(args, logMessage);

    // Assemble log line:
    logPrintSerialLine(LOG_ERROR, "🟥", logTag, logMessage, args);

    va_end(args);
}

void logInfo(const char *logTag, const char *logMessage, ...) {
    // Handle variable arguments:
    va_list args;
    va_start(args, logMessage);

    // Assemble log line:
    logPrintSerialLine(LOG_INFO, "🟩", logTag, logMessage, args);

    va_end(args);
}

void logWarning(const char *logTag, const char *logMessage, ...) {
    // Handle variable arguments:
    va_list args;
    va_start(args, logMessage);

    // Assemble log line:
    logPrintSerialLine(LOG_WARN, "🟨", logTag, logMessage, args);

    va_end(args);
}

void logVerbose(const char *logTag, const char *logMessage, ...) {
    // Handle variable arguments:
    va_list args;
    va_start(args, logMessage);

    // Assemble log line:
    logPrintSerialLine(LOG_VERBOSE, "⬛", logTag, logMessage, args);

    va_end(args);
}

void logSetLevel(LogLevel level) {
    const char *logTag = __func__;

    logInfo(logTag, "Setting log level to %d", level);

    // Update global log level
    logLevelGlobal = level;
}
