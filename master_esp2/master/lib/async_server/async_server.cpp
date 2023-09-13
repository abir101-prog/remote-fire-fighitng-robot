#include "ESPAsyncWebServer.h"
#include "ArduinoJson.h"
#include "async_server.h"
#include "string.h"



AsyncWebServer async_server(81);   // change port to 81 while running 2 servers simulatenoely

void setup_async_server() {
    async_server.on("/start", HTTP_GET, [](AsyncWebServerRequest *request){  // starts the esp to look around
    idle = false;
    // jhakanaka show
    // digitalWrite(FLASH_LIGHT, LOW);
    // delay(1000);
    // digitalWrite(FLASH_LIGHT, HIGH);
    delay(100);
    request->send(200, "text/plain", "Starting the robot");
  });

    async_server.on("/fire", HTTP_POST, [](AsyncWebServerRequest * request){}, NULL,
            [](AsyncWebServerRequest * request, uint8_t *data, size_t len, size_t index, size_t total) { // sends to esp if there is fire
    // Serial.println("POST request received");
    StaticJsonDocument<100> responsebuffer; //JSONBuffer;
    deserializeJson(responsebuffer, data);
    JsonObject obj = responsebuffer.as<JsonObject>();
    // Serial.print(response);
    if (obj["fire"]) {
      fire = true;
      int centerx = obj["centerx"];
      int img_width = obj["img_width"];
      if (servo_angle==0) Serial.write(1);
      else Serial.write(servo_angle);

      // fire should be in middle 60% of image space
      // if(servo_angle==0) {
      //   if (img_width * 0.2 > centerx) Serial.write(1); // fire in left 20%
      //   else if (centerx > img_width * 0.8) Serial.write(3);  // right
      //   else Serial.write(2);
      // }  // if totalstep!=0, 2 bytes will be sent only
      delay(50); // so that esp doesnt keep sending too many command to

    } else fire = false;

    
    
    request->send(200, "text/plain", "received fire alert");

    // process the received data (center X position of fire)

  });



  async_server.begin();
}