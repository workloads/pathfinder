#include "HttpServer.h"

// Static member initialization
WebServer *HttpServer::httpServer = nullptr;

bool HttpServer::init() {
    const char *logTag = __func__;

    if (!httpServer) {
        httpServer = new WebServer(httpServerPort);
        Logger::info(logTag, "HTTP server initialized on port `%d`", httpServerPort);

        return true;
    } else {
        Logger::error(logTag, "HTTP server pointer is null, initialization failed");

        return false;
    }
}

void HttpServer::handleEvents() {
    const char *logTag = __func__;

    if (httpServer) {
        httpServer->handleClient();
        Logger::info(logTag, "HTTP server handling client interactions");

        mapRoutes();
    } else {
        Logger::error(logTag, "HTTP server not initialized");
    }
}

void HttpServer::handleErrorBadRequest() {
    const char *logTag = __func__;

    httpServer->send(400, "text/plain", "400: Bad Request");
}

void HttpServer::handleErrorForbidden() {
    const char *logTag = __func__;

    httpServer->send(403, "text/plain", "403: Forbidden");
}

void HttpServer::handleErrorNotFound() {
    const char *logTag = __func__;

    httpServer->send(404, "text/plain", "404: Not Found");
}

void HttpServer::handleGetRoot() {
    const char *logTag = __func__;

    httpServer->send(200, "text/plain", "root");
}

void HttpServer::handleGetV1DeviceStatus() {
    const char *logTag = __func__;

    httpServer->send(200, "text/plain", "handleGetV1DeviceStatus");
}

void HttpServer::handleGetV1DeviceBattery() {
    const char *logTag = __func__;

    httpServer->send(200, "text/plain", "handleGetV1DeviceBattery");
}

void HttpServer::handleGetV1DeviceReboot() {
    const char *logTag = __func__;

    httpServer->send(200, "text/plain", "handleGetV1DeviceReboot");
}


void HttpServer::mapRoutes() {
    const char *logTag = __func__;

    if (httpServer) {
        // Handle requests to root
        httpServer->on("/", HTTP_GET, handleGetRoot);

        httpServer->on("/v1/device/status", HTTP_GET, handleGetV1DeviceStatus);
        httpServer->on("/v1/device/battery", HTTP_GET, handleGetV1DeviceBattery);
        httpServer->on("/v1/device/reboot", HTTP_POST, handleGetV1DeviceReboot);

        // Handle 404 not found errors
        httpServer->onNotFound(handleErrorNotFound);

        httpServer->begin();
        Logger::info(logTag, "HTTP server routes mapped and server started on port `%d`", httpServerPort);
    } else {
        Logger::error(logTag, "HTTP server pointer is null, cannot map routes");
    }
}

bool HttpServer::streamFile(WebServer *server, const char *filename, const char *contentType) {
    const char *logTag = __func__;

    File file = Filesystem::streamFile(filename, "r");

    if (!file) {
        Logger::error(logTag, "Failed to open file `%s`", filename);

        return false;
    }

    server->streamFile(file, contentType);
    file.close();

    Logger::info(logTag, "File streamed successfully `%s`", filename);

    return true;
}
