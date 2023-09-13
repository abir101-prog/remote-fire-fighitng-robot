#include <Arduino.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX pins for SoftwareSerial
void setup() {
  Serial.begin(115200); // Initialize the hardware serial port for debugging
  Serial.println("The code started uno\n");
  mySerial.begin(115200); // Initialize the software serial port
}
void loop() {
  if (mySerial.available()) {  // received data available
    char data[100]; 
    size_t len = Serial.readBytesUntil('\n', data, 15);
    data[len] = '\n';
    data[len+1] = '\0';
    Serial.println(len);

    // char* data_c = strcat(data, "\n");
    Serial.println(data);
    delay(100);
    mySerial.write(data);

  } else Serial.print("uno myserial not available\n");
  delay(1000);
}