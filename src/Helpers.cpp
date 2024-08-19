#include "Helpers.h"

#include "Log.h"

bool helperCheckIfStringStartsWithProtocol(const char *input) {
    return strncmp(input, "http", 4) == 0;
}

const char *helperIpAddressToString(IPAddress inputAddr) {
    static char output[16];
    snprintf(output, sizeof(output), "%d.%d.%d.%d", inputAddr[0], inputAddr[1], inputAddr[2], inputAddr[3]);
    Log::info(__func__, "IP Address: %s", output);
    return output;
}

int helperStringToLogLevel(const char *levelString) {
    const LogLevelMap *logLevels     = Log::getLogLevels();
    int                logLevelsSize = Log::getLogLevelsSize();

    for (int i = 0; i < logLevelsSize; i++) {
        if (strcmp(logLevels[i].name, levelString) == 0) {
            Log::info(__func__, "Result: %d", logLevels[i].level);
            return logLevels[i].level;
        }
    }

    return -1;
}

uint8_t helperPercentageToInteger(int percentage) {
    const char *logTag = __func__;

    if (percentage < 0) {
        Log::warning(logTag, "Percentage outside lower bound, setting to 0");
        percentage = 0;
    } else if (percentage > 100) {
        Log::warning(logTag, "Percentage outside upper bound, setting to 100");
        percentage = 100;
    }

    return (uint8_t)((percentage * 255) / 100);
}
