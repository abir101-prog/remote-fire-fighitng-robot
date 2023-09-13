#include <Arduino.h>

#include <PS2MouseHandler.h>

#define MOUSE_DATA 10 //5
#define MOUSE_CLOCK 11 //6

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

    Serial.print(mouse.x_movement()); // X Movement Data
    Serial.print(",");
    Serial.print(mouse.y_movement()); // Y Movement Data

    Serial.println();
  }
}
