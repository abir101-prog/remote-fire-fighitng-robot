#include "string.h"
#include <Arduino.h>

char received[40];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  // char msg[] = "hello abir\n";
  // Serial.write(msg);
  // delay(5);

}
void loop() {
  //Serial.println("Message Received: ");
  
  if (Serial.available() > 0){   
    Serial.readBytesUntil('\n', received, 39);
    delay(3000);
    Serial.write(received); 
  }
  delay(5);


}