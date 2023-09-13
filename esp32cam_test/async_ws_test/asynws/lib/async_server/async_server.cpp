#include "ESPAsyncWebServer.h"
#include "ArduinoJson.h"

AsyncWebServer server(81);   // change port to 81 while running 2 servers simulatenoely

void setup_async_server() {
    server.on("/hello", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(200, "text/plain", "Hello World");
  });

    server.on("/data", HTTP_POST, [](AsyncWebServerRequest * request){}, NULL,
            [](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) {
    Serial.println("POST request received");
    StaticJsonDocument<100> JSONBuffer;
    deserializeJson(JSONBuffer, data);
    JsonObject obj = JSONBuffer.as<JsonObject>();
    String response = obj["key1"];
    Serial.print(response);
    
    request->send(200, "text/plain", "faisi");
  });



  server.begin();
}