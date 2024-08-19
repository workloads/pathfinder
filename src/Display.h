#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <Adafruit_SSD1306.h>  // http://librarymanager/All/Display#Adafruit_SSD1306
#include <Arduino.h>
#include <Wire.h>

#include "Log.h"

/**
 * @brief Display class for managing OLED display initialization and updates.
 */
class Display {
 private:
    // Display height and width (in pixels)
    constexpr static uint8_t displayHeight = 128;
    constexpr static uint8_t displayWidth  = 32;

    // Display text color and size
    constexpr static uint16_t displayTextColor = SSD1306_WHITE;
    constexpr static uint8_t  displayTextSize  = 1;

    // Display Reset Pin (or `-1` if shared with Arduino Reset Pin)
    constexpr static int8_t displayResetPin = -1;

    // Display address
    constexpr static uint8_t displayAddress = 0x3C;

    // Initialize Display Library
    static Adafruit_SSD1306 display;

 public:
    /**
     * @brief Initializes the OLED display.
     *
     * @return True if the display was successfully initialized, false otherwise.
     */
    static bool init();

    /**
     * @brief Updates the OLED display with the provided screen lines.
     *
     * @param lines Array of strings to be displayed on the screen.
     */
    static void update(const char *lines[4]);
};

#endif  // DISPLAY_H_
