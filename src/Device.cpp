#include "Device.h"

// Internal configuration variables
char Device::applicationCompilationString[32];

// Toggle to sleep HTTP Server
const bool Device::enableHttpServer = true;

// Toggle to enable ESP Insights
const bool Device::enableInsights = false;

// Toggle to enable configuration changes via serial interface
const bool Device::enableSerialConfiguration = true;

char *Device::identifier(int length) {
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

bool Device::setup() {
    const char *logTag = __func__;

    // Store compilation time string
    snprintf(applicationCompilationString, sizeof(applicationCompilationString), "%s at %s", __DATE__, __TIME__);

    // Print to serial monitor directly as loggers are not yet initialized
    Serial.println();
    Serial.println("----------------------------------------------------------------");

    char initialLogMessage[96];
    snprintf(initialLogMessage, sizeof(initialLogMessage), "[%s][🏁 START] Initializing device", logTag);
    Serial.println(initialLogMessage);

    // Explicitly set the log level
    Logger::setLevel(static_cast<LogLevel>(CORE_DEBUG_LEVEL));

    // TODO(ksatirli): Initialize Battery
    // Battery::init();

    // Create an instance of WifiClient
    WifiClient wifiClient;

    // Scan for Wi-Fi networks
    wifiClient.scanNetworks();

    // Wi-Fi and other initialization code
    if (!wifiClient.disconnect()) {
        Logger::error(logTag, "Failed to disconnect previous Wi-Fi connection");
        return false;
    }

    if (!wifiClient.connect()) {
        Logger::error(logTag, "Failed to establish a Wi-Fi connection");
        return false;
    }

    if (!HttpServer::init()) {
        Logger::error(logTag, "Failed to initialize HTTP server");
        return false;
    }

    if (!Display::init()) {
        Logger::error(logTag, "Failed to initialize Display");
        return false;
    }

    if (!WireClient::init()) {
        Logger::error(logTag, "Failed to initialize Wire");
        return false;
    }

    // if (enableInsights) {
    //    insightsInit(applicationInsightsKey);
    // }

    // Indicate that setup was successful
    return true;
}

void Device::loop() {
    const char *logTag = __func__;

    if (enableSerialConfiguration) {
        Logger::debug(logTag, "Handling serial input events");
        // Serial::handleEvents();
    }

    if (enableHttpServer) {
        Logger::debug(logTag, "Handling HTTP Server events");
        HttpServer::handleEvents();
    }

    // Update display
    const char *lines[4] = {"Line 1", "Line 2", "Line 3", "Line 4"};
    Display::update(lines);
}

void Device::information() {
    const char *logTag = __func__;

    // TODO(ksatirli): Add code here to gather and display device information
}
