#include <Arduino.h>

// #include <ps2.h>

/*
 * Pin 5 is the mouse data pin, pin 6 is the clock pin
 * Feel free to use whatever pins are convenient.
 */
// #define MOUSE_DATA 5
// #define MOUSE_CLK 6

// void mouse_init();
// void setup();
// void loop();
// PS2 mouse(MOUSE_CLK, MOUSE_DATA);

// /*
//  * initialize the mouse. Reset it, and place it into remote
//  * mode, so we can get the encoder data on demand.
//  */
// void mouse_init()
// {
//   mouse.write(0xff);  // reset
//   Serial.println("write done");
//   mouse.read();  // ack byte
//   Serial.println("read done");
//   mouse.read();  // blank */
//   mouse.read();  // blank */
//   Serial.println("read done");
//   mouse.write(0xf0);  // remote mode
//   Serial.println("write done");
//   mouse.read();  // ack
//   delayMicroseconds(100);
// }

// void setup()
// {
//   Serial.begin(9600);
//   Serial.println("starting...");
//   mouse_init();
//   Serial.println("Initialization complete");
// }


// void loop()
// {
//   char mstat;
//   char mx;
//   char my;

//   /* get a reading from the mouse */
//   mouse.write(0xeb);  // give me data!
//   mouse.read();      // ignore ack
//   mstat = mouse.read();
//   mx = mouse.read();
//   my = mouse.read();

//   /* send the data back up */
//   Serial.print(mstat, BIN);
//   Serial.print("\tX=");
//   Serial.print(mx, DEC);
//   Serial.print("\tY=");
//   Serial.print(my, DEC);
//   Serial.println();
//   delay(500);  /* twiddle */
// }


void setup() {
  Serial.begin(115200);
}
void loop() {
  Serial.println("Hello");
  delay(100);
}