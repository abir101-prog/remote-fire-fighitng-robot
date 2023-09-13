#include <Arduino.h>
#include <SoftwareSerial.h>

#define rxPin 10
#define txPin 11

// Set up a new SoftwareSerial object
SoftwareSerial mySerial =  SoftwareSerial(rxPin, txPin);

void setup()  {
  // Define pin modes for TX and RX
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  Serial.begin(9600);
  // Set the baud rate for the SoftwareSerial object
  mySerial.begin(9600);
}
int i= 0;
void loop() {
  if (i == 0){
    delay(5000);
    const char imu_data[] = "{\"posx\":10,\"posy\":-30}\n";  // sample json data
    mySerial.write(imu_data); 
    i++;
  }

  if (mySerial.available() > 0) {
    char received[30];
    mySerial.readBytesUntil('\n', received, 29);
    Serial.println(received);
  }
}