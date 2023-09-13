#include <Arduino.h>

const int trigpPin = 9;
const int echoPin = 10;

long duration;
int distance;

void setup() {
    pinMode(trigpPin, OUTPUT);
    pinMode(echoPin, INPUT);
    Serial.begin(9600);
}

void loop() {
    digitalWrite(trigpPin, LOW);
    daleyMicroseconds(2);

    digitalWrite(trigpPin, HIGH);
    daleyMicroseconds(10);
    digitalWrite(trigpPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    distance = duration * 0.034/2;

    Serial.print("distance = ");
    Serial.println(distance);
    delayMicroseconds(1000);
}
