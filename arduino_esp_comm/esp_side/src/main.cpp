#include <Arduino.h>
// void setup() {
//   Serial.begin(115200); // Initialize the hardware serial port
// }
// void loop() {
//   if (Serial.available()) {
//     char data = 'a';
//     // char data = Serial.read();
//     // Process the received data here
//     // Echo back the data to the serial port
//     // Serial.print(data);
//     Serial.write(data);
//     char received = Serial.read();
//     Serial.print(received);
//   } else Serial.print('Serial is not availabe');
// }

// this sample code provided by www.programmingboss.com
// #include <SoftwareSerial.h>
// #include <HardwareSerial.h>

// HardwareSerial mySerial(15, 14); // RX, TX


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("The code started esp\n");
  // mySerial.begin(115200);
  Serial1
}

bool sent = false;

void loop() {
  delay(100);
  char* msg = "hello abir\n";
  mySerial.write(msg, 11);

  delay(100);

  if (mySerial.available()) {
    char data[100] ="";
    size_t len = mySerial.readBytesUntil('\0', data, 15);
    // char* data_c = strcat(data, "\n");
    data[len] = '\0';
    Serial.println(data);
    mySerial.write(data);

    delay(100);

  } else Serial.print("esp mySerial nai esp te\n");
  delay(1000);
}

