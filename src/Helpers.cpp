#include "Helpers.h"

#include "Log.h"


/**
 * @brief Checks if a string starts with a protocol.
 *
 * This function checks if the input string starts with a protocol, such as `http` or `https`.
 *
 * @param input The string to check.
 *
 * @return `true` if the string starts with a protocol, `false` otherwise.
 */
inline bool helperCheckIfStringStartsWithProtocol(const char *input) {
    // Check if input starts with "http" (includes "https")
    return strncmp(input, "http", 4) == 0;
}

/**
 * @brief Converts IP Address of type IPAddress to a string.
 *
 * @param inputAddr The IP Address to convert to a string.
 *
 * @return The string representation of the IP Address.
 */
inline const char *helperIpAddressToString(IPAddress inputAddr) {
    static char output[16];  // Static storage duration to return safely, 15 characters + null terminator
    snprintf(output, sizeof(output), "%d.%d.%d.%d", inputAddr[0], inputAddr[1], inputAddr[2], inputAddr[3]);

    logInfo(__func__, "IP Address: %s", output);

    return output;
}

/**
 * @brief Converts a string representation of a log level to its corresponding numeric value.
 *
 * @param levelString The string representation of the log level.
 *
 * @return The numeric value of the log level, or -1 if the input string is invalid.
 */
inline int helperStringToLogLevel(const char *levelString) {
    for (int i = 0; i < logLevelsSize; i++) {
        if (strcmp(logLevels[i].name, levelString) == 0) {
            logInfo(__func__, "Result: %d", logLevels[i].level);
            return logLevels[i].level;
        }
    }

    // Unknown log level string
    return -1;
}

/**
 * @brief Converts percentage to an integer in the range of 0 to 255.
 *
 * This function takes a percentage value (0-100) and converts it to
 * an integer value (0-255), scaling the percentage to fit the 8-bit
 * range. If the input percentage is outside the 0-100 range, it will
 * be clamped to ensure the return value stays within bounds.
 *
 * @param percentage The percentage to convert to an integer. Expected range is 0 to 100.
 * @return The integer value of the percentage, scaled to 0-255.
 */
inline uint8_t helperPercentageToInteger(int percentage) {
    const char *logTag = __func__;

    if (percentage < 0) {
        logWarning(logTag, "Percentage outside lower bound, setting to 0");
        percentage = 0;
    } else if (percentage > 100) {
        logWarning(logTag, "Percentage outside upper bound, setting to 100");
        percentage = 100;
    }

    // Calculate integer from percentage
    return (uint8_t)((percentage * 255) / 100);
}
