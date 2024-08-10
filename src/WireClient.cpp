#include "WireClient.h"

#include "Log.h"

////////////////////////////////////////////////////////////////////////////////
// CONFIGURATION                                                              //
////////////////////////////////////////////////////////////////////////////////

constexpr int S_SCL = 33;
constexpr int S_SDA = 32;
////////////////////////////////////////////////////////////////////////////////

bool wireInit() {
    const char *logTag = __func__;

    // See https://www.arduino.cc/reference/en/language/functions/communication/wire/begin/
    if (!Wire.begin(S_SDA, S_SCL)) {
        logError(logTag, "Failed to initialize Wire");

        return false;
    }

    return true;
}
