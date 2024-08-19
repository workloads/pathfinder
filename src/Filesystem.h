#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_

#include <LittleFS.h>  // http://librarymanager/All/Data%20Storage#LittleFS_esp32

/**
 * @brief Filesystem class for managing LittleFS initialization and file operations.
 */
class Filesystem {
 public:
    /**
     * @brief Initialize the filesystem.
     *
     * This function initializes the LittleFS filesystem.
     *
     * @return True if the filesystem was successfully initialized, false otherwise.
     */
    static bool init();

    /**
     * @brief Stream a file from the filesystem.
     *
     * This function opens a file from the filesystem in the specified mode.
     *
     * @param filename Name of the file to stream.
     * @param mode Mode in which to open the file.
     * @return A File object representing the opened file.
     */
    static File streamFile(const String &filename, const char *mode);
};

#endif  // FILESYSTEM_H_
