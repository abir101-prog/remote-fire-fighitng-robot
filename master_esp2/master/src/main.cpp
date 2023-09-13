#include <Arduino.h>
#include <WebServer.h>
#include <WiFi.h>
#include <esp32cam.h>
#include <video_server.h>
#include <async_server.h>
#include <pwmWrite.h>


Pwm pwm = Pwm();

// put function declarations here:
int FLASH_LIGHT = 4;
#define RED_LED 33  // active low
// stepper motor
const int servo_pin = 12;  // Coil A wire 1

const char* WIFI_SSID = "wifi name";
const char* WIFI_PASS = "pass";
 
static auto loRes = esp32cam::Resolution::find(320, 240);
static auto midRes = esp32cam::Resolution::find(350, 530);
static auto hiRes = esp32cam::Resolution::find(800, 600);


// idle = true: ghum, false: alert
bool idle = true;
bool fire = false;
int servo_angle = 90;  // 1-179 degree(ghuruk pura ta but 0 degree arduino te 1degree hisebe jabe jodi lage)
int curr_step = 1;  // 1=right, -1=left



// flags
bool isConnected = false;  // wifi connnected or not
 
void  setup(){
  idle = true;
  fire = false;
  // pinMode(servo_pin, OUTPUT);
  // cam_servo.attach(servo_pin);
  pinMode(RED_LED, OUTPUT);
  digitalWrite(RED_LED, HIGH);

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
  pwm.writeServo(servo_pin, 90);
  delay(1000);
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

  if (idle) { return; }   // loop() will be called again

  ///////////////// NON IDLE ZONE   //////////////////////////
  // if in alert mode (received fire info)
  // look for fire : rotate 360 till fire is found
  if (fire) {
    // tell arduino to turn towards fire (send step number) (done in on server response)
    delay(5);
    servo_angle = 90;
    pwm.writeServo(servo_pin, servo_angle);
    delay(300);
    fire = false;
  } else {
    servo_angle += 10 * curr_step;
    pwm.writeServo(servo_pin, servo_angle);
    delay(300);
    if (servo_angle==180) curr_step = -1;
    if (servo_angle==0) curr_step = 1;
  }
  
}

