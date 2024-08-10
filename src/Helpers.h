#ifndef HELPERS_H_
#define HELPERS_H_

#include <Arduino.h>
#include <IPAddress.h>

/**
 * @brief Checks if a string starts with a protocol.
 *
 * This function checks if the input string starts with a protocol, such as `http` or `https`.
 *
 * @param input The string to check.
 *
 * @return `true` if the string starts with a protocol, `false` otherwise.
 */
bool helperCheckIfStringStartsWithProtocol(const char *input);
inline bool helperCheckIfStringStartsWithProtocol(const char *input);

/**
 * @brief Converts IP Address of type IPAddress to a string.
 *
 * @param inputAddr The IP Address to convert to a string.
 *
 * @return The string representation of the IP Address.
 */
inline const char *helperIpAddressToString(IPAddress inputAddr);

/**
 * @brief Converts a string representation of a log level to its corresponding numeric value.
 *
 * @param levelString The string representation of the log level.
 *
 * @return The numeric value of the log level, or -1 if the input string is invalid.
 */
inline int helperStringToLogLevel(const char *levelString);

/**
 * @brief Converts percentage to an integer in the range of 0 to 255.
 *
 * This function takes a percentage value (0-100) and converts it to
 * an integer value (0-255), scaling the percentage to fit the 8-bit
 * range. If the input percentage is outside the 0-100 range, it will
 * be clamped to ensure the return value stays within bounds.
 *
 * @param percentage The percentage to convert to an integer. Expected range is 0 to 100.
 * @return The integer value of the percentage, scaled to 0-255.
 */
inline uint8_t helperPercentageToInteger(int percentage);

#endif  // HELPERS_H_
