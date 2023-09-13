#include <Arduino.h>

// Define the pins for controlling the stepper motor
const int coil_A1 = 12;  // Coil A wire 1
const int coil_A2 = 13;  // Coil A wire 2
const int coil_B1 = 14;  // Coil B wire 1
const int coil_B2 = 15;  // Coil B wire 2
int total_count = 0;


void stepper(float angle, int inc);
void step(int direction);

void setup() {
  // Set the coil pins as outputs
  Serial.begin(115200);
  pinMode(coil_A1, OUTPUT);
  pinMode(coil_A2, OUTPUT);
  pinMode(coil_B1, OUTPUT);
  pinMode(coil_B2, OUTPUT);
}

void loop() {

  stepper(90, 1);
  Serial.println(total_count);
  delay(1000); // Pause for a moment
}

void step(int direction) {
  static int sequence[][4] = {{1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1}};// {0, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 1, 1}, {0, 0, 0, 1}};
  static int currentStep = 0;

  currentStep += direction;

  // Ensure currentStep stays within 0 to 7 for the sequence array
  if (currentStep < 0) {
    currentStep = 3;
  } else if (currentStep > 3) {
    currentStep = 0;
  }

  // Set the coil pins based on the sequence
  digitalWrite(coil_A1, sequence[currentStep][0]);
  digitalWrite(coil_A2, sequence[currentStep][1]);
  digitalWrite(coil_B1, sequence[currentStep][2]);
  digitalWrite(coil_B2, sequence[currentStep][3]);
}

void stepper(float angle, int inc) {
  float step_angle =  2040.0 / 360;
  int num_steps = angle * step_angle;
  // Serial.println(num_steps);

  // Rotate clockwise for one full revolution
  for (int i = 0; i < num_steps; i++) {
    step(inc);  // Step forward
    delay(5); // Adjust this delay to control the motor speed
  } 
  total_count += num_steps;
  total_count %= 2040;
}
