#ifndef HTTPSERVER_H_
#define HTTPSERVER_H_

#include <WebServer.h>

/**
 * @brief Global pointer to the HTTP server instance.
 */
extern WebServer *httpServer;

/**
 * @brief Initialize and configure the HTTP server.
 *
 * This function sets up the HTTP server, registers request handlers, and starts the server.
 */
void httpServerInit();

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
bool httpServerStreamFile(WebServer *server, const char *filename, const char *contentType = "text/html");

/**
 * @brief Begin processing incoming HTTP requests.
 *
 * This function handles incoming HTTP requests and processes them.
 */
void httpServerHandleEvents();

/**
 * @brief Handle requests to the root ("/") path.
 *
 * This function handles HTTP GET requests to the root path.
 */
void httpServerHandlerOnConnect();

/**
 * @brief Handle requests to undefined paths.
 *
 * This function handles HTTP requests to paths that are not defined.
 */
void httpServerHandlerNotFound();

#endif  // HTTPSERVER_H_
