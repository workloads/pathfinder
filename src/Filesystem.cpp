#include "Filesystem.h"

#include <LittleFS.h>

#include "Log.h"

bool filesystemInit() {
    const char *logTag = __func__;

    if (!LittleFS.begin()) {
        logError(logTag, "Unable to mount filesystem");
        return false;
    }

    logInfo(logTag, "Successfully mounted filesystem");
    return true;
}

File filesystemStreamFile(const String &filename, const char *mode) {
    const char *logTag = __func__;

    File file = LittleFS.open(filename, mode);

    if (!file) {
        logError(logTag, ("Failed to open file: " + filename).c_str());
    }

    return file;
}
