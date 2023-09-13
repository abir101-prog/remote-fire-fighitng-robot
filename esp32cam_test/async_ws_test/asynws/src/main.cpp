#include <Arduino.h>
#include "WiFi.h"
// #include "ESPAsyncWebServer.h"  included inside httpserver
#include "async_server.h"

const char* ssid = "Helikopter Helikopter 🚁";
const char* password = "6251=Heda";
const int RED_LED = 33;

void setup() {
  pinMode(RED_LED, OUTPUT);
  Serial.begin(115200);
 
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
      delay(1000);
      Serial.println("Connecting to WiFi..");
  }
  if (WiFi.isConnected()) 
  Serial.println(WiFi.localIP());
  
  setup_async_server();  // all server routes are here

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

