#include "Display.h"

// Initialize the display object with the specified width, height, and reset pin
Adafruit_SSD1306 Display::display(Display::displayWidth, Display::displayHeight, &Wire, Display::displayResetPin);

bool Display::init() {
    const char *logTag = __func__;

    // `SSD1306_SWITCHCAPVCC` = generate display voltage from 3.3V internally
    if (!display.begin(SSD1306_SWITCHCAPVCC, displayAddress)) {
        Log::error(logTag, "SSD1306 allocation failed");
        return false;
    }

    Log::debug(logTag, "Clearing Display");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextColor(displayTextColor);
    display.setTextSize(displayTextSize);

    Log::debug(logTag, "Rendering Image Buffer");
    display.display();

    return true;
}

void Display::update(const char *lines[4]) {
    const char *logTag = __func__;

    Log::debug(logTag, "Clearing display");
    display.clearDisplay();
    display.setCursor(0, 0);

    for (int i = 0; i < 4; ++i) {
        display.println(lines[i]);
    }

    Log::debug(logTag, "Rendering lines");
    display.display();
}
