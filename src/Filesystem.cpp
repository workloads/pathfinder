#include "Filesystem.h"

#include <Logger.h>

bool Filesystem::init() {
    const char *logTag = __func__;

    if (!LittleFS.begin()) {
        Logger::error(logTag, "Unable to mount filesystem");
        return false;
    }

    Logger::info(logTag, "Successfully mounted filesystem");
    return true;
}

File Filesystem::streamFile(const String &filename, const char *mode) {
    const char *logTag = __func__;

    File file = LittleFS.open(filename, mode);

    if (!file) {
        Logger::error(logTag, ("Failed to open file: " + filename).c_str());
    }

    return file;
}
