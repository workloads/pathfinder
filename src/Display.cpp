#include "Display.h"

#include "Log.h"

////////////////////////////////////////////////////////////////////////////////
// CONFIGURATION                                                              //
////////////////////////////////////////////////////////////////////////////////

// Display height and width (in pixels)
constexpr uint8_t displayHeight = 128;
constexpr uint8_t displayWidth  = 32;

// Display text color and size
// See https://github.com/adafruit/Adafruit_SSD1306/blob/master/Adafruit_SSD1306.h for color overview
constexpr uint16_t displayTextColor = SSD1306_WHITE;
constexpr uint8_t  displayTextSize  = 1;

// Display Reset Pin (or `-1` if shared with Arduino Reset Pin)
constexpr int8_t displayResetPin = -1;

// Display address
//             128 x 32 display = `0x3C`
//             128 x 64 display = `0x3D`
constexpr uint8_t displayAddress = 0x3C;

// Initialize display library
Adafruit_SSD1306 display(displayWidth, displayHeight, &Wire, displayResetPin);

bool displayInit() {
    const char *logTag = __func__;

    // `SSD1306_SWITCHCAPVCC` = generate display voltage from 3.3V internally
    if (!display.begin(SSD1306_SWITCHCAPVCC, displayAddress)) {
        logError(logTag, "SSD1306 allocation failed");

        return false;
    }

    logDebug(logTag, "Clearing Display");
    display.clearDisplay();
    display.setCursor(0, 0);
    display.setTextColor(displayTextColor);
    display.setTextSize(displayTextSize);

    logDebug(logTag, "Rendering Image Buffer");
    display.display();

    return true;
}

void displayUpdate(const char *lines[4]) {
    const char *logTag = __func__;

    logDebug(logTag, "Clearing display");
    display.clearDisplay();
    display.setCursor(0, 0);

    for (int i = 0; i < 4; ++i) {
        display.println(lines[i]);
    }

    logDebug(logTag, "Rendering lines");
    display.display();
}
