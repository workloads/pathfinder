#include "Device.h"

#include "Display.h"
#include "HttpServer.h"
// #include "Insights.h"  // TODO(ksatirli) re-enable when upstream warning is fixed
#include "Log.h"
#include "WifiClient.h"
#include "WireClient.h"

////////////////////////////////////////////////////////////////////////////////
// CONFIGURATION                                                              //
////////////////////////////////////////////////////////////////////////////////

char applicationCompilationString[32];

// TODO(ksatirli) move this to a configuration file
// see https://dashboard.insights.espressif.com/home/manage-auth-keys/
const char applicationInsightsKey[] = "";

// Toggle to sleep HTTP Server
const bool enableHttpServer = true;

// Toggle to enable ESP Insights
// See https://dashboard.insights.espressif.com/
const bool enableInsights = false;

// Toggle to enable configuration changes via serial interface
const bool enableSerialConfiguration = true;
////////////////////////////////////////////////////////////////////////////////

char *deviceIdentifier(int length) {
    const char *logTag = __func__;

    // Temporary buffer to hold the full UniqueID8 as a hex string
    // 8 bytes * 2 characters per byte + 1 for null terminator
    char fullIdentifier[17];
    int  index = 0;

    for (int i = 0; i < 8; i++) {
        if (UniqueID8[i] < 0x10) {
            fullIdentifier[index++] = '0';
        }

        // Calculate remaining buffer size
        int remainingBufferSize = sizeof(fullIdentifier) - index;

        // Format the current byte in UniqueID8 as lowercase hexadecimal string
        snprintf(&fullIdentifier[index], remainingBufferSize, "%02x", UniqueID8[i]);
        index += 2;
    }

    // Ensure null termination of the full identifier
    fullIdentifier[16] = '\0';

    // Calculate start index to get the last `length` characters
    int startIndex = (16 > length) ? (16 - length) : 0;

    // Dynamically allocate memory for the identifier
    char *identifier = new char[length + 1];

    // Copy the last `length` characters from the full identifier
    strncpy(identifier, &fullIdentifier[startIndex], length);

    // Ensure null termination
    identifier[length] = '\0';

    return identifier;
}

bool deviceSetup() {
    const char *logTag = __func__;

    // Store compilation time string
    snprintf(applicationCompilationString, sizeof(applicationCompilationString), "%s at %s", __DATE__, __TIME__);

    // Print to serial monitor directly as loggers are not yet initialized
    Serial.println();
    Serial.println('----------------------------------------------------------------');

    char initialLogMessage[64];
    snprintf(initialLogMessage, sizeof(initialLogMessage), "[%s][🏁 START] Initializing device", logTag);
    Serial.println(initialLogMessage);

    // Explicitly set the log level
    logSetLevel(static_cast<LogLevel>(CORE_DEBUG_LEVEL));

    // Scan for Wi-Fi networks
    wifiScanNetworks();

    // Wi-Fi and other initialization code
    if (!wifiDisconnect()) {
        logError(logTag, "Failed to disconnect previous Wi-Fi connection");

        return false;
    }

    if (!wifiConnect()) {
        logError(logTag, "Failed to establish a Wi-Fi connection");

        return false;
    }

    if (!httpServerInit()) {
        logError(logTag, "Failed to initialize HTTP server");

        return false;
    }

    if (!displayInit()) {
        logError(logTag, "Failed to initialize Display");

        return false;
    }

    if (!wireInit()) {
        logError(logTag, "Failed to initialize Wire");

        return false;
    }

    // if (enableInsights) {
    //    insightsInit(applicationInsightsKey);
    // }

    // Indicate that setup was successful
    return true;
}

void deviceLoop() {
    const char *logTag = __func__;

    if (enableSerialConfiguration) {
        logDebug(logTag, "Handling serial input events");
        //        serialHandleEvents();
    }

    if (enableHttpServer) {
        logDebug(logTag, "Handling HTTP Server events");
        httpServerHandleEvents();
    }

    // Update display
    const char *lines[4] = {"Line 1", "Line 2", "Line 3", "Line 4"};
    displayUpdate(lines);
}

void deviceInformation() {
    const char *logTag = __func__;

    // TODO(ksatirli) add code here
}
