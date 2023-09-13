#include <Arduino.h>
#include <WebServer.h>
#include <WiFi.h>
#include <esp32cam.h>
#include <video_server.h>
#include <async_server.h>

void stepper(int inc);
void step(int direction);


// put function declarations here:
int FLASH_LIGHT = 4;
#define RED_LED 33  // active low
// stepper motor
const int coil_A1 = 12;  // Coil A wire 1
const int coil_A2 = 13;  // Coil A wire 2
const int coil_B1 = 14;  // Coil B wire 1
const int coil_B2 = 15;  // Coil B wire 2


const char* WIFI_SSID = "your wifi";
const char* WIFI_PASS = "pass";
 
static auto loRes = esp32cam::Resolution::find(320, 240);
static auto midRes = esp32cam::Resolution::find(350, 530);
static auto hiRes = esp32cam::Resolution::find(800, 600);


// idle = true: ghum, false: alert
bool idle = true;
bool fire = false;
int total_step_count = 0;  // tracks number of steps it crossed (2pi <-> 2040)
int curr_step = 1;  // 120 degree cross korle -1


// flags
bool isConnected = false;  // wifi connnected or not
 
void  setup(){
  idle = true;
  fire = false;
  pinMode(coil_A1, OUTPUT);
  pinMode(coil_A2, OUTPUT);
  pinMode(coil_B1, OUTPUT);
  pinMode(coil_B2, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  digitalWrite(RED_LED, HIGH);
  // while(true) stepper(-1);

  Serial.begin(115200); // to commmunicate with arduino
  Serial.println();
  {
    using namespace esp32cam;
    Config cfg;
    cfg.setPins(pins::AiThinker);
    cfg.setResolution(hiRes);
    cfg.setBufferCount(2);
    cfg.setJpeg(80);
 
    bool ok = Camera.begin(cfg);
    Serial.println(ok ? "CAMERA OK" : "CAMERA FAIL");
  }

  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  print_link();  // link to access photo
 
  video_stream();     // video access
  setup_async_server();  // all async server routes are here(data)

}
 


void loop()
{
    // diode will be on when wifi is connected, blink if not
  if (WiFi.status() == WL_CONNECTED && !isConnected) {
    // Serial.println("Connected");
    digitalWrite(RED_LED, LOW);
    isConnected = true;
  }

  if (WiFi.status() != WL_CONNECTED) {
    // Serial.println(".");
    digitalWrite(RED_LED, !digitalRead(RED_LED));
    isConnected = false;   
    delay(1000);
  }
  stream_client();   // video stream

  if (idle) return;   // loop() will be called again

  ///////////////// NON IDLE ZONE   //////////////////////////
  // if in alert mode (received fire info)
  // look for fire : rotate 360 till fire is found
  if (fire) {
    // tell arduino to turn towards fire (send step number) (done in on server response)
    delay(5);
    // bring stepper to its initial pose
    while(total_step_count != 0) {
      if (total_step_count < 0) stepper(-1);
      else stepper(1);
    }
  } else {
    stepper(curr_step); // after every 68 steps, stream client will listen for call
    delay(1000);
    if (abs(total_step_count) > 600) curr_step*= -1;
    Serial.println(curr_step);
  }
  
}



void step(int direction) { // +1: CW. -1:CCW
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

void stepper(int inc) {  // steps 68 times in each call
  int num_steps = 68; //angle * step_angle;

  for (int i = 0; i < num_steps; i++) {
    step(inc);  // Step forward
    delay(5); // Adjust this delay to control the motor speed
  } 
  total_step_count += num_steps * inc;  // dan hole + bam -
  total_step_count %= 2040;
}
