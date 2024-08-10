#include "HttpServer.h"

#include <WebServer.h>

#include "Filesystem.h"
#include "Log.h"

////////////////////////////////////////////////////////////////////////////////
// CONFIGURATION                                                              //
////////////////////////////////////////////////////////////////////////////////

// `7284` is `PATH` on a phone keypad
const uint16_t httpServerPort = 7284;

// Declare a pointer for the HTTP server for dynamic allocation
WebServer *httpServer = nullptr;
////////////////////////////////////////////////////////////////////////////////

bool httpServerInit() {
    const char *logTag = __func__;

    if (!httpServer) {
        httpServer = new WebServer(httpServerPort);
        logInfo(logTag, "HTTP server initialized on port `%d`", httpServerPort);

        return true;
    } else {
        logError(logTag, "HTTP server pointer is null, initialization failed");

        return false;
    }
}

void httpServerMapRoutes() {
    const char *logTag = __func__;

    // TODO(ksatirli) dynamically populate these entries from a config
    if (httpServer) {
        // Handle requests to root
        httpServer->on("/", HTTP_GET, httpServerHandlerOnConnect);

        // Handle 404 not found errors
        httpServer->onNotFound(httpServerHandlerNotFound);

        httpServer->begin();
        logInfo(logTag, "HTTP server routes mapped and server started on port `%d`", httpServerPort);
    } else {
        logError(logTag, "HTTP server pointer is null, cannot map routes");
    }
}

bool httpServerStreamFile(WebServer *server, const char *filename, const char *contentType) {
    const char *logTag = __func__;

    File file = filesystemStreamFile(filename, "r");

    if (!file) {
        logError(logTag, "Failed to open file `%s`", filename);

        return false;
    }

    server->streamFile(file, contentType);
    file.close();

    logInfo(logTag, "File streamed successfully `%s`", filename);

    return true;
}

void httpServerHandleEvents() {
    const char *logTag = __func__;

    if (httpServer) {
        httpServer->handleClient();
        logInfo(logTag, "HTTP server handling client interactions");
    } else {
        logError(logTag, "HTTP server not initialized");
    }
}

void httpServerHandlerOnConnect() {
    const char *logTag = __func__;

    if (!httpServerStreamFile(httpServer, "/index.html", "text/html")) {
        httpServer->send(500, "text/plain", "Internal Server Error");
    }
}

void httpServerHandlerNotFound() {
    const char *logTag = __func__;

    httpServer->send(404, "text/plain", "404: Not Found");
}
