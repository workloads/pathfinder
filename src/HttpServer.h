#ifndef HTTPSERVER_H_
#define HTTPSERVER_H_

#include "./web_page.h"

// Create AsyncWebServer object on port 80
WebServer server(80);

void httpServerHandleRoot() {
  server.send(200, "text/html", index_html);  // Send web page
}

void httpServerController() {
  server.on("/", httpServerHandleRoot);

  server.on("/js", []() {
    String jsonCmdWebString = server.arg(0);
    deserializeJson(jsonCmdReceive, jsonCmdWebString);
    jsonCmdReceiveHandler();
    serializeJson(jsonInfoHttp, jsonFeedbackWeb);
    server.send(200, "text/json", jsonFeedbackWeb);
    jsonFeedbackWeb = "";
    jsonInfoHttp.clear();
    jsonCmdReceive.clear();
  });

  // Start server
  server.begin();
  Serial.println("Server Starts.");
}

void httpServerInit() {
  httpServerController();
}

#endif  // HTTPSERVER_H_
