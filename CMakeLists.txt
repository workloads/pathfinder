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

# set common paths
# see https://cmake.org/cmake/help/v3.30/command/set.html
# ⚠️ If this value is updated, it MUST be updated project-wide
# (arduino-cli and Arduino IDE default directories)
set(ARDUINO_DATA_PATH $ENV{HOME}/Library/Arduino15/packages)
set(ARDUINO_LIBS_PATH $ENV{HOME}/Documents/Arduino/libraries)

# set Arduino library version
# see https://cmake.org/cmake/help/v3.30/command/set.html
set(ARDUINO_LIBRARY_VERSION 1.8.6)

# set ESP32 library version
# ⚠️ If this value is updated, it MUST be updated project-wide
# see https://cmake.org/cmake/help/v3.30/command/set.html
set(ESP32_LIBRARY_VERSION 3.3.12)

# see https://cmake.org/cmake/help/v3.30/command/include_directories.html
include_directories(
    ${ARDUINO_LIBS_PATH}
    ${ARDUINO_DATA_PATH}/arduino/hardware/avr/${ARDUINO_LIBRARY_VERSION}/libraries
    ${ARDUINO_DATA_PATH}/arduino/hardware/avr/${ARDUINO_LIBRARY_VERSION}/cores/arduino
    ${ARDUINO_DATA_PATH}/arduino/hardware/avr/${ARDUINO_LIBRARY_VERSION}/variants/standard
    ${ARDUINO_DATA_PATH}/esp32/hardware/esp32/${ESP32_LIBRARY_VERSION}/libraries
    ${ARDUINO_DATA_PATH}/esp32/hardware/esp32/${ESP32_LIBRARY_VERSION}/cores/esp32
    ${ARDUINO_DATA_PATH}/esp32/hardware/esp32/${ESP32_LIBRARY_VERSION}/variants/esp32
)

# include all source files in the `src` directory
# see https://cmake.org/cmake/help/v3.30/command/file.html
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

# see https://cmake.org/cmake/help/v3.30/command/add_executable.html
add_executable(pathfinder pathfinder.ino ${SOURCES})

# explicitly specify language as CXX (C++)
# see https://cmake.org/cmake/help/v3.30/command/set_target_properties.html
set_target_properties(pathfinder PROPERTIES LINKER_LANGUAGE CXX)
