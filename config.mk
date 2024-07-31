# project-specific configuration for Pathfinder

# Note that Pathfinder currently depends on `v2` of the ESP32 core, which is not the latest version.
# see # see https://github.com/espressif/arduino-esp32
ARDUINO_BOARD_CORE ?= "arduino:esp32@2.0.13"

# fully qualified board name (FQBN) for the target board
# see https://arduino.github.io/arduino-cli/latest/commands/arduino-cli-board-list/
ARDUINO_BOARD_FQBN ?= "esp32:esp32:esp32"

ARDUINO_BOARD_PROGRAMMER ?= "esptool"

# list of upstream Arduino libraries to be installed
ARDUINO_LIBRARIES_UPSTREAM ?= \
    "Adafruit BusIO@1.16.1" \
    "Adafruit GFX Library@1.11.10" \
    "Adafruit ICM20X@2.0.7" \
    "Adafruit Unified Sensor@1.1.14" \
    "Adafruit SSD1306@2.5.11" \
    "ArduinoJson@7.1.0" \
    "ESP32Encoder@0.11.6" \
    "ICM20948_WE@1.1.8" \
    "INA219_WE@1.3.8" \
    "LittleFS esp32@1.0.6" \
    "PID v2@2.0.1" \
    "SimpleKalmanFilter@0.1"

# list of unsafe / non-upstream Arduino libraries to be installed
ARDUINO_LIBRARIES_UNSAFE ?= \
	"https://github.com/workloads/scservo.git"

