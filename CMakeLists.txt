# see https://cmake.org/cmake/help/v3.30/command/cmake_minimum_required.html
cmake_minimum_required(VERSION 3.30.0)

# see https://cmake.org/cmake/help/v3.30/command/project.html#project
project(
    pathfinder
    DESCRIPTION "ESP32-based mobile edge computing platform"
    HOMEPAGE_URL "https://go.workloads.io/pathfinder"
    LANGUAGES CXX
)

# see https://cmake.org/cmake/help/latest/variable/CMAKE_LANG_CLANG_TIDY.html
set(CMAKE_CXX_CLANG_TIDY clang-tidy -checks=-*,readability-*)

# see https://docs.espressif.com/projects/esp-idf/en/v3.30/esp32s3/api-guides/cplusplus.html
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# generate `compile_commands.json` for use with `clangd`
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ⚠️ If this value is updated, it MUST be updated project-wide
set(ARDUINO_BASE_PATH .arduino)

# set Arduino and ESP32 library versions
# ⚠️ If these values are updated, they MUST be updated project-wide
set(ARDUINO_LIBRARY_VERSION 1.8.6)
set(ESP32_LIBRARY_VERSION 2.0.17)

# see https://cmake.org/cmake/help/v3.30/command/include_directories.html
include_directories(
    SYSTEM ${ARDUINO_BASE_PATH}/data/packages/arduino/hardware/avr/${ARDUINO_LIBRARY_VERSION}/cores/arduino
    SYSTEM ${ARDUINO_BASE_PATH}/data/packages/arduino/hardware/avr/${ARDUINO_LIBRARY_VERSION}/libraries
    SYSTEM ${ARDUINO_BASE_PATH}/data/packages/esp32/hardware/esp32/${ESP32_LIBRARY_VERSION}/cores/esp32
    SYSTEM ${ARDUINO_BASE_PATH}/user/libraries
    SYSTEM ${ARDUINO_BASE_PATH}/data/packages/esp32/hardware/esp32/${ESP32_LIBRARY_VERSION}/libraries
)

# include all source files in the `src` directory
# see https://cmake.org/cmake/help/v3.30/command/file.html
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/*.h")

# see https://cmake.org/cmake/help/v3.30/command/add_executable.html
add_executable(pathfinder pathfinder.ino ${SOURCES})

# ignore warning about literal suffixes, specifically for `libraries/Insights/src/Insights.cpp:8`
# see https://cmake.org/cmake/help/v3.30/command/add_compile_options.html
add_compile_options(-Wno-literal-suffix)

# explicitly specify language as CXX (C++)
# see https://cmake.org/cmake/help/v3.30/command/set_target_properties.html
set_target_properties(pathfinder PROPERTIES LINKER_LANGUAGE CXX)
