#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include <LittleFS.h>

/**
 * @brief Initialize the filesystem.
 *
 * This function initializes the LittleFS filesystem.
 *
 * @return True if the filesystem was successfully initialized, false otherwise.
 */
bool filesystemInit();

/**
 * @brief Stream a file from the filesystem.
 *
 * This function opens a file from the filesystem in the specified mode.
 *
 * @param filename Name of the file to stream.
 * @param mode Mode in which to open the file.
 * @return A File object representing the opened file.
 */
File filesystemStreamFile(const String& filename, const char* mode);

#endif  // FILESYSTEM_H_
