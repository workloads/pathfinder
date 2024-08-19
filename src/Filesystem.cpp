#include "Filesystem.h"

#include "Log.h"

bool Filesystem::init() {
    const char *logTag = __func__;

    if (!LittleFS.begin()) {
        Log::error(logTag, "Unable to mount filesystem");
        return false;
    }

    Log::info(logTag, "Successfully mounted filesystem");
    return true;
}

File Filesystem::streamFile(const String &filename, const char *mode) {
    const char *logTag = __func__;

    File file = LittleFS.open(filename, mode);

    if (!file) {
        Log::error(logTag, ("Failed to open file: " + filename).c_str());
    }

    return file;
}
