#include <Arduino.h>
// #include <PS2MouseHandler.h>

// #define MOUSE_DATA 5
// #define MOUSE_CLOCK 6


// PS2MouseHandler mouse(MOUSE_CLOCK, MOUSE_DATA, PS2_MOUSE_REMOTE);

// void setup() {
//   pinMode(MOUSE_CLOCK, INPUT);
//   pinMode(MOUSE_DATA, INPUT);
//   Serial.begin(115200);
//   if (mouse.initialise() != 0) Serial.println("Mouse didn't initialize properly");
//   else { 
//     Serial.println("initialized!");
//   }
// }

// unsigned long last_run = millis();

// void loop() {
//   int16_t x = mouse.x_movement(); // X Movement Data
//   int16_t y = mouse.y_movement(); // Y Movement Data

//   if (millis() - last_run > 200) {
//     mouse.get_data();  // gets the stored data in mouse, after sending, mouse starts counting from 0
//     // Serial.println(mouse.status());
//     int16_t x = mouse.x_movement(); // X Movement Data (-255,255)
//     // int16_t y = mouse.y_movement(); 
//     Serial.print(mouse.y_movement());
//   }
  
// }

#include <PS2MouseHandler.h>
#define MOUSE_DATA 5
#define MOUSE_CLOCK 6

PS2MouseHandler mouse(MOUSE_CLOCK, MOUSE_DATA, PS2_MOUSE_REMOTE);

/**
 * Setup
 */
void setup()
{
  Serial.begin(115200);
  if(mouse.initialise() != 0){
    // mouse error
    Serial.println("mouse error");
  };

  Serial.println(mouse.device_id());
}

/**
 * Main program loop
 */

unsigned long last_run = millis();

void loop()
{
  if (millis() - last_run > 200) {
    last_run = millis();
    mouse.get_data();
    Serial.print(mouse.device_id()); // device id
    Serial.print(":");
    Serial.print(mouse.status()); // Status Byte
    Serial.print(":");
    Serial.print(mouse.x_movement()); // X Movement Data
    Serial.print(",");
    Serial.print(mouse.y_movement()); // Y Movement Data

    Serial.println();
  }
}
