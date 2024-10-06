#ifndef DEVICE_H_
#define DEVICE_H_

#include <Arduino.h>
#include <ArduinoUniqueID.h>  // http://librarymanager/All/Other#ArduinoUniqueID
#include <esp_system.h>

#include "Display.h"
#include "HttpServer.h"
#include <Logger.h>
#include "WifiClient.h"
#include "WireClient.h"

/**
 * @brief Device class for managing device-specific operations such as setup, loop, and information retrieval.
 */
class Device {
 private:
    static char applicationCompilationString[32];

    // Toggle to sleep HTTP Server
    static const bool enableHttpServer;

    // Toggle to enable ESP Insights
    static const bool enableInsights;

    // Toggle to enable configuration changes via serial interface
    static const bool enableSerialConfiguration;

    // Delay (in msec) how long to delay Device Setup
    constexpr static int deviceSetupBlockDelay = 5000;

 public:
    /**
     * @brief Retrieves device identifier.
     *
     * This function retrieves a unique device identifier and formats it for downstream use.
     *
     * @param length The length of the string to return.
     * @return The text representation of the device identifier.
     */
    static char *identifier(int length = 16);

    /**
     * @brief Initializes the `setup` stage.
     *
     * @return `true` if setup was successful, `false` otherwise.
     */
    static bool setup();

    /**
     * @brief Executes the `loop` stage.
     */
    static void loop();

    /**
     * @brief Gathers device information from ESP32 module.
     */
    static void information();
};

#endif  // DEVICE_H_
