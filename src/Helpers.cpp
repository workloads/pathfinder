#include "Helpers.h"

#include "Log.h"

inline bool helperCheckIfStringStartsWithProtocol(const char *input) {
    // Check if input starts with "http" (includes "https")
    return strncmp(input, "http", 4) == 0;
}

inline const char *helperIpAddressToString(IPAddress inputAddr) {
    // 15 characters + null terminator
    static char output[16];
    snprintf(output, sizeof(output), "%d.%d.%d.%d", inputAddr[0], inputAddr[1], inputAddr[2], inputAddr[3]);

    logInfo(__func__, "IP Address: %s", output);

    return output;
}

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
