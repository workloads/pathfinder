// Create AsyncWebServer object on port 80
WebServer server(80);

#if WEB_PAGE
  // serve the web UI from LittleFS (see `data/index.html` and `task upload-fs`)
  void handleRoot() {
    File file = LittleFS.open("/index.html", "r");
    if (!file) {
      server.send(404, "text/plain", "/index.html not found on LittleFS");
      return;
    }
    server.streamFile(file, "text/html");
    file.close();
  }
#endif

void webCtrlServer() {
  #if WEB_PAGE
    server.on("/", handleRoot);
    server.serveStatic("/style.css", LittleFS, "/style.css");
  #endif

  server.on("/js", []() {
    String jsonCmdWebString = server.arg("json");
    deserializeJson(jsonCmdReceive, jsonCmdWebString);
    jsonCmdReceiveHandler();
    // jsonFeedbackWeb is populated by the base feedback commands;
    // other commands (such as `{"T":405}`) only fill jsonInfoHttp
    if (jsonFeedbackWeb.length() == 0 && jsonInfoHttp.size() > 0) {
      serializeJson(jsonInfoHttp, jsonFeedbackWeb);
    }
    server.send(200, "application/json", jsonFeedbackWeb);
    jsonFeedbackWeb = "";
    jsonInfoHttp.clear();
    jsonCmdReceive.clear();
  });

  server.on("/v1/device/status", []() {
    server.send(200, "application/json", deviceStatusFeedback());
  });

  #if API_V1
  server.on("/v1", []() {
    server.send(200, "text/plain", "/v1");
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
    int statusCode;
    String result = apiMovementPlanHandler(server.arg("plain"), statusCode);
    server.send(statusCode, "text/plain", result);
 });
 #endif
 #if API_V2
 server.on("/v2/movement", HTTP_GET, []() {
    server.send(200, "text/plain", getSteps());
  });
  server.on("/v2/movement", HTTP_POST, []() {
    server.send(200, "text/plain", apiv2MovementPlanHandler(server.arg("plain")));
  });
  server.on("/v2/movement/run", HTTP_POST, []() {
    server.send(200, "text/plain", runSteps());
  });
 #endif

  // Start server
  server.begin();
  Serial.println("Server Starts.");
}

void initHttpWebServer() {
  webCtrlServer();
}