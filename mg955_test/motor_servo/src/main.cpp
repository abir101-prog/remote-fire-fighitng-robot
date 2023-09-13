#include <Arduino.h>
// #include <Servo.h>

// Servo myservo;
#define pump 10

// put function declarations here:
int pos = 0;    // variable to store the servo position

void setup() {
  // myservo.attac
  Serial.begin(115200);
  pinMode(pump, OUTPUT);  // attaches the servo on pin 9 to the servo object
  // myservo.write(90);

}

void loop() {
  Serial.println("Helo");
  digitalWrite(pump, LOW);
  delay(5000);
  digitalWrite(pump, HIGH);
  delay(1000);
  // for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
  //   // in steps of 1 degree
  //   myservo.write(pos);              // tell servo to go to position in variable 'pos'
  //   delay(15);                       // waits 15 ms for the servo to reach the position
  // }
  // for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
  //   myservo.write(pos);              // tell servo to go to position in variable 'pos'
  //   delay(15);                       // waits 15 ms for the servo to reach the position
  // }
}