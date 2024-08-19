#include "HttpServer.h"

// Static member initialization
WebServer *HttpServer::httpServer = nullptr;

bool HttpServer::init() {
    const char *logTag = __func__;

    if (!httpServer) {
        httpServer = new WebServer(httpServerPort);
        Log::info(logTag, "HTTP server initialized on port `%d`", httpServerPort);

        return true;
    } else {
        Log::error(logTag, "HTTP server pointer is null, initialization failed");

        return false;
    }
}

void HttpServer::mapRoutes() {
    const char *logTag = __func__;

    if (httpServer) {
        // Handle requests to root
        httpServer->on("/", HTTP_GET, handlerOnConnect);

        // Handle 404 not found errors
        httpServer->onNotFound(handlerNotFound);

        httpServer->begin();
        Log::info(logTag, "HTTP server routes mapped and server started on port `%d`", httpServerPort);
    } else {
        Log::error(logTag, "HTTP server pointer is null, cannot map routes");
    }
}

bool HttpServer::streamFile(WebServer *server, const char *filename, const char *contentType) {
    const char *logTag = __func__;

    File file = Filesystem::streamFile(filename, "r");

    if (!file) {
        Log::error(logTag, "Failed to open file `%s`", filename);

        return false;
    }

    server->streamFile(file, contentType);
    file.close();

    Log::info(logTag, "File streamed successfully `%s`", filename);

    return true;
}

void HttpServer::handleEvents() {
    const char *logTag = __func__;

    if (httpServer) {
        httpServer->handleClient();
        Log::info(logTag, "HTTP server handling client interactions");
    } else {
        Log::error(logTag, "HTTP server not initialized");
    }
}

void HttpServer::handlerOnConnect() {
    const char *logTag = __func__;

    if (!streamFile(httpServer, "/index.html", "text/html")) {
        httpServer->send(500, "text/plain", "Internal Server Error");
    }
}

void HttpServer::handlerNotFound() {
    const char *logTag = __func__;

    httpServer->send(404, "text/plain", "404: Not Found");
}
