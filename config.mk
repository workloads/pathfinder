# project-specific configuration for Pathfinder

# timeout for the board discovery process
ARDUINO_CLI_COMPILE_DISCOVERY_TIMEOUT = "1s"

# number of parallel jobs to use for compiling the sketch
ARDUINO_CLI_COMPILE_JOBS ?= "0"

# output directory for compiled artifacts
ARDUINO_CLI_COMPILE_OUTPUT_DIRECTORY = "./build"

# protocol to use for compiling the sketch
ARDUINO_CLI_COMPILE_PROTOCOL = "serial"

# warning level for the compile process
ARDUINO_CLI_COMPILE_WARNINGS = "default"

# baudrate for the serial monitor
ARDUINO_CLI_MONITOR_BAUDRATE ?= "115200"

# board core and version to be used for compiling the sketch
ARDUINO_CORE ?= "esp32:esp32@2.0.17"

# list of upstream Arduino libraries to be installed
# see https://arduino.github.io/arduino-cli/1.0/library-specification/
ARDUINO_LIBRARIES_UPSTREAM ?= \
    "Adafruit BusIO@1.16.1" \
    "Adafruit GFX Library@1.11.10" \
    "Adafruit ICM20X@2.0.7" \
    "Adafruit SSD1306@2.5.11" \
    "Adafruit Unified Sensor@1.1.14" \
    "ArduinoJson@7.1.0" \
    "ArduinoUniqueID@1.3.0" \
    "ESP32Encoder@0.11.6" \
    "ICM20948_WE@1.1.8" \
    "INA219_WE@1.3.8" \
    "LittleFS_esp32@1.0.6" \
    "PID_v2@2.0.1" \
    "SimpleKalmanFilter@0.1"

# list of unsafe / non-upstream Arduino libraries to be installed
ARDUINO_LIBRARIES_UNSAFE ?= \
	"https://github.com/workloads/scservo.git"

# path to the Arduino sketch configuration file
ARDUINO_SKETCH_CONFIG ?= "config.yaml"

# path to the (primary) Arduino sketch file
ARDUINO_SKETCH_FILE ?= "pathfinder.ino"

# profile to be used for compiling the sketch
ARDUINO_SKETCH_PROFILE ?= "default"