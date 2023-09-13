#include <Arduino.h>
#include "ESPAsyncWebServer.h"
#include <WiFi.h>
#include <esp32cam.h>
#include "async_server.h"

// put function declarations here:
#define FLASH_LIGHT 4
#define RED_LED 33  // active low

const char* WIFI_SSID = "Helikopter Helikopter 🚁";
const char* WIFI_PASS = "6251=Heda";
 
void setup() {
  pinMode(RED_LED, OUTPUT);
  Serial.begin(115200);
 
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  
  while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi..");
  }
  if (WiFi.isConnected()) 
  Serial.println(WiFi.localIP());
  
  server_setup();  // all server routes are here

}

// flags
bool isConnected = false;

void loop() {
    // diode will be on when wifi is connected, blink if not
  if (WiFi.status() == WL_CONNECTED && !isConnected) {
    Serial.println("Connected");
    digitalWrite(RED_LED, LOW);
    isConnected = true;
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println(".");
    digitalWrite(RED_LED, !digitalRead(RED_LED));
    isConnected = false;   
    delay(1000);
  }
}

