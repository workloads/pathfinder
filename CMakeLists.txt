# see https://cmake.org/cmake/help/v3.30/command/cmake_minimum_required.html
cmake_minimum_required(VERSION 3.30.0)

# see https://cmake.org/cmake/help/v3.30/command/project.html#project
project(
        pathfinder
        DESCRIPTION "ESP32-based mobile edge computing platform"
        HOMEPAGE_URL "https://go.workloads.io/pathfinder"
        LANGUAGES CXX
)

# see https://docs.espressif.com/projects/esp-idf/en/v3.30/esp32s3/api-guides/cplusplus.html
# and https://cmake.org/cmake/help/v3.30/command/set.html
set(CMAKE_CXX_STANDARD 11)

# generate `compile_commands.json` for use with `clangd`
# see https://cmake.org/cmake/help/v3.30/command/set.html
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set base path and common version strings
set(ARDUINO_BASE_PATH       $ENV{HOME}/Library/Arduino15/packages)
set(ARDUINO_LIBRARY_VERSION 1.8.6)

# ⚠️ If this value is updated here, it MUST also be updated in `config.mk` and `sketch.yaml`.
set(ESP32_LIBRARY_VERSION 2.0.17)

# see https://cmake.org/cmake/help/v3.30/command/include_directories.html
include_directories(
        ${ARDUINO_BASE_PATH}/arduino/hardware/avr/${ARDUINO_LIBRARY_VERSION}/cores/arduino
        ${ARDUINO_BASE_PATH}/esp32/hardware/esp32/${ESP32_LIBRARY_VERSION}/libraries/ESPmDNS/src
        ${ARDUINO_BASE_PATH}/esp32/hardware/esp32/${ESP32_LIBRARY_VERSION}/libraries/HTTPClient/src
        ${ARDUINO_BASE_PATH}/esp32/hardware/esp32/${ESP32_LIBRARY_VERSION}/libraries/WiFi/src
        ${ARDUINO_BASE_PATH}/esp32/hardware/esp32/${ESP32_LIBRARY_VERSION}/libraries/WiFiClientSecure/src
)

# include all source files in the `src` directory
# see https://cmake.org/cmake/help/v3.30/command/file.html
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

# see https://cmake.org/cmake/help/v3.30/command/add_executable.html
add_executable(pathfinder pathfinder.ino ${SOURCES})

# explicitly specify language as CXX (C++)
# see https://cmake.org/cmake/help/v3.30/command/set_target_properties.html
set_target_properties(pathfinder PROPERTIES LINKER_LANGUAGE CXX)
