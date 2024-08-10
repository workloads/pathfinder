#ifndef DEVICE_H_
#define DEVICE_H_

#include <Arduino.h>
#include <ArduinoUniqueID.h>
#include <esp_system.h>

/**
 * @brief Retrieves device identifier
 *
 * This function retrieves a unique device identifier and formats it for downstream use
 *
 * @param length The length of the string to return.
 *
 * @return The text representation of the device identifier
 */
char *deviceIdentifier(int length = 16);

/**
 * @brief Initializes the `setup` stage.
 */
bool deviceSetup();

/**
 * @brief Executes the `loop` stage.
 */
void deviceLoop();

/**
 * @brief Gathers device information from ESP32 module.
*/
void deviceInformation();

#endif  // DEVICE_H_
