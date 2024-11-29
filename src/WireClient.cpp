#include "WireClient.h"

#include <Logger.h>

bool WireClient::init() {
    const char *logTag = __func__;

    // See https://www.arduino.cc/reference/en/language/functions/communication/wire/begin/
    if (!Wire.begin(S_SDA, S_SCL)) {
        Logger::error(logTag, "Failed to initialize Wire");
        return false;
    }

    return true;
}
