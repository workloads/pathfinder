#ifndef HTTPSERVER_H_
#define HTTPSERVER_H_

#include <WebServer.h>

#include "Filesystem.h"
#include "Log.h"

/**
 * @brief HttpServer class for managing HTTP server initialization, route mapping, and request handling.
 */
class HttpServer {
 private:
    constexpr static uint16_t httpServerPort = 7284;

    // Pointer to the HTTP server instance
    static WebServer *httpServer;

    /**
     * @brief Handle requests to the root ("/") path.
     *
     * This function handles HTTP GET requests to the root path.
     */
    static void handlerOnConnect();

    /**
     * @brief Handle requests to undefined paths.
     *
     * This function handles HTTP requests to paths that are not defined.
     */
    static void handlerNotFound();

 public:
    /**
     * @brief Initialize and configure the HTTP server.
     *
     * This function sets up the HTTP server, registers request handlers, and starts the server.
     *
     * @return True if the server was successfully initialized, false otherwise.
     */
    static bool init();

    /**
     * @brief Map routes for the HTTP server.
     *
     * This function registers the routes and handlers for the HTTP server.
     */
    static void mapRoutes();

    /**
     * @brief Stream a file over HTTP.
     *
     * This function streams a file to the client over HTTP.
     *
     * @param server Pointer to the WebServer instance.
     * @param filename Name of the file to stream.
     * @param contentType Content type of the file.
     * @return True if the file was successfully streamed, false otherwise.
     */
    static bool streamFile(WebServer *server, const char *filename, const char *contentType = "text/html");

    /**
     * @brief Begin processing incoming HTTP requests.
     *
     * This function handles incoming HTTP requests and processes them.
     */
    static void handleEvents();
};

#endif  // HTTPSERVER_H_
