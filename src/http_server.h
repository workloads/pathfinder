#include "web_page.h"

// Create AsyncWebServer object on port 80
WebServer server(80);

void handleRoot() {
  server.send(200, "text/html", index_html);
}

void webCtrlServer() {
  server.on("/", handleRoot);

  server.on("/js", []() {
    String jsonCmdWebString = server.arg("json");
    deserializeJson(jsonCmdReceive, jsonCmdWebString);
    jsonCmdReceiveHandler();
    // jsonFeedbackWeb is populated by the command handler
    server.send(200, "application/json", jsonFeedbackWeb);
    jsonFeedbackWeb = "";
    jsonInfoHttp.clear();
    jsonCmdReceive.clear();
  });

  server.on("/v1", []() {
    server.send(200, "text/plain", "/v1");
  });

  server.on("/v1/device/status", []() {
    server.send(200, "application/json", deviceStatusFeedback());
  });

  server.on("/v1/device/battery", []() {
    server.send(200, "application/json", batteryStatusFeedback());
  });

  server.on("/v1/device/reboot", []() {
    server.send(200, "text/plain", "rebooting...");
    esp_restart();
  });

  server.on("/v1/device/wifi", []() {
    server.send(200, "application/json", wifiStatusFeedback());
  });

  server.on("/v1/device/wifi/scan", []() {
    server.send(200, "text/json", "/v1/device/wifi/scan");
  });

  server.on("/v1/healthz", []() {
    server.send(200, "text/plain", "v1/healthz");
  });

  server.on("/v1/readyz", []() {
    server.send(200, "text/plain", "/v1/readyz");
  });

  server.on("/v1/stop", []() {
    server.send(200, "text/plain", apiStopDevice());
  });

  server.on("/v1/direct-command", HTTP_POST, []() {
    server.send(200, "text/plain", jsonAPIHandler(server.arg("plain")));
  });

  server.on("/v1/movement-plan", HTTP_POST, []() {
    server.send(200, "text/plain", apiMovementPlanHandler(server.arg("plain")));
 });


  // Start server
  server.begin();
  Serial.println("Server Starts.");
}

void initHttpWebServer() {
  webCtrlServer();
}