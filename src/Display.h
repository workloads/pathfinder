#ifndef DISPLAY_H_
#define DISPLAY_H_

// Built-in and Third-Party libraries
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <Wire.h>

/**
 * @brief Initializes the OLED display.
 */
bool displayInit();

/**
 * @brief Updates the OLED display with the provided screen lines.
 *
 * @param lines array of strings to be displayed on the screen.
 */
void displayUpdate(const char *lines[4]);

#endif  // DISPLAY_H_
