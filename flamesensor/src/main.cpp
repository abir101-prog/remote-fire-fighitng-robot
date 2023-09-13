#include <Arduino.h>

// put function declarations here:
int anal = A4;
int dig = A5;

void setup() {
  // put your setup code here, to run once:
  pinMode(anal, INPUT);
  pinMode(dig, INPUT);
  Serial.begin(115200);
}

void loop() {
  Serial.println("Started");
  if (digitalRead(dig) == HIGH) Serial.println("Fire!");
  else Serial.println("no fire");
  Serial.println(analogRead(anal));

  delay(1000);








}

