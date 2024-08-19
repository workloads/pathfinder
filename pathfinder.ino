/*
 *
 * @file pathfinder.ino
 * @brief ESP32-based mobile edge computing platform
 * @url https://go.workloads.io/pathfinder
 * @version 0.10.0
 *
*/

////////////////////////////////////////////////////////////////////////////////
// CONFIGURATION                                                              //
////////////////////////////////////////////////////////////////////////////////

// How many milliseconds to delay before restarting `setup()`
const int deviceSetupRestartDelay = 3000;

// Baud rate to use for serial communication
const int serialBaudRate = 115200;
////////////////////////////////////////////////////////////////////////////////

// Load local configuration and helpers
#include "src/Device.h"
#include "src/Log.h"

// Setup application flow
void setup() {
    const char *logTag = __func__;

    // Initialize serial communication
    Serial.begin(serialBaudRate);
    Serial.println("Serial connection initialized");

    Log::debug(logTag, "Initializing device setup");

    while (!Device::setup()) {
        Log::warning(logTag, "Device setup failed, retrying in %d seconds...", deviceSetupRestartDelay / 1000);  // NOLINT(whitespace/line_length)

        // Delay restart of `setup()` so device and ecosystem (e.g.: Wi-Fi APs)
        // have enough time to process operations and restore connectivity
        // see https://www.arduino.cc/reference/en/language/functions/time/delay/
        delay(deviceSetupRestartDelay);
    }
}

// Enter application flow
void loop() {
    Device::loop();
}
