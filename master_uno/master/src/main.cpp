#include <Arduino.h>
#include <Servo.h>
#include <SoftwareSerial.h>

#define rxPin 12
#define txPin 13
SoftwareSerial esp_serial =  SoftwareSerial(rxPin, txPin);
// DELAYS OF MECHANICAL ACTUALTORS MIGHT HAVE TO BE REPLACED BY MILLIS

Servo US_servo;  // ultrasound - water pipe - analog flame sensor 

// put function declarations here:
void moveForward();
void moveBackward();
void hault();
void turnLeft();
void turnRight();
float distanceMeasure();
void put_off_fire();
void avoid_obstacle();
bool locate_fire();  // direct toward fire sensed by flame sensor: return 1 if located or 0 if not found
// bool fire_detected();


// pin labeling
int in1 =2;
int in2=4;
int enA =3;

int in3 =5;
int in4 = 7;
int enB = 6;

// ultrasound pins
int trigPin = 9;
int echoPin = 8;  // analog?

int pump = 10; 

int servo_pin = 11;

// flame sensor pins  : using 1 analog sensor for now
// int fs_left = 12;
// int fs_right = 13;
int fs_front = A5;

// internal variables
// US servo
int us_servo_pos = 0;

//fire detection
int fire = A4; // digital pin of flame sensor 
const int fs_threshold = 800;  // for analog input sensor



void setup() {
  Serial.begin(115200);
  esp_serial.begin(115200);  // might have to change
  // serial comm with esp
  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);
  // motor driver control (for wheels)
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  // ultrasound sensor
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);
  // pump
  pinMode(pump, OUTPUT);
  digitalWrite(pump, HIGH); // maybe inverter
  // servo initial position
  US_servo.attach(servo_pin);
  US_servo.write(90); 
  
  delay(10000);

}

// this 2 data(koto angle ghora lagbe(cw) or olpo kondike ghora lagbe (if steps=0 dir 0 or 2))
// if step=0 and dir > 2 data is invalid
int stepper_steps = 0;
int direction = 5; // initially eta matter korena cz 5 (1 2 3) (l f r)
bool been_ordered = false;  // ordered to move by esp?

void loop() {
  // listen to esp32cam
  if (esp_serial.available()){
    int count = 0;  // takes the last 2-3 bytes (latest) data by checking initial bytes
    int init0 = esp_serial.read();
    while (count < 63) {
      int init1 = esp_serial.read();
      if ((init0==255) & (init1==255)) {
        if (esp_serial.available() <= 3) break;
        else init0 = init1;
      }
    }
    int sgn = esp_serial.read();
    int b1 = esp_serial.read();
    int b0 = esp_serial.read();
    stepper_steps = 256*b1+b0;
    if (sgn) stepper_steps *= -1;
    Serial.println(stepper_steps);
    been_ordered =true;
    if (stepper_steps==0) {
      direction = esp_serial.read();
      Serial.println(direction);  //////// check to see if data has come to uno
    }
  } else been_ordered = false;

  if (!been_ordered) {
    hault();
    delay(500);
  } else {
    // fire anywhre?
    int located = locate_fire();  //detect and turn the robot in that direction

    if (located) put_off_fire();

    // manuever
    float distance = distanceMeasure();
    if (distance < 10) avoid_obstacle();
    else {
      if (stepper_steps == 0) {
        if(direction==2) { moveForward(); delay(300); }
        else if (direction==1) { turnLeft(); delay(100); } // delays has to be calibrated  
        else if (direction==3) { turnRight(); delay(100); } 

      } else if (stepper_steps > 0) { // cw
          turnRight(); delay(300);

      } else {
          turnLeft(); delay(300);
      }
    }
  }
  been_ordered = false;



}


// bool fire_detected() {
  // int sensor_data = analogRead(fs_front);
  // Serial.println(sensor_data);
  // if (sensor_data < fs_threshold) fire = true;
  // else fire = false;
  // return fire;
// }

bool locate_fire() {  // angle of view might need to be extended
  // 1st check at center:
  if (digitalRead(fire)) {
    hault();
    delay(200);
    return true;
  }
  for (us_servo_pos = 90; us_servo_pos <= 130; us_servo_pos += 5) { 
    US_servo.write(us_servo_pos); 
    delay(50); 
    if (digitalRead(fire)) {
      turnRight();
      delay(200);
      hault();
      US_servo.write(90);
      return true;
    }
  }
  for (us_servo_pos = 130; us_servo_pos >= 50; us_servo_pos -= 5) { 
    US_servo.write(us_servo_pos); 
    delay(50);
    if (digitalRead(fire)) {
      turnLeft();
      delay(200);
      hault();
      US_servo.write(90);
      return true;
    }
  }
  for (us_servo_pos = 50; us_servo_pos <= 90; us_servo_pos += 5) { 
    US_servo.write(us_servo_pos);
    delay(50);
  }
  return false;
}


float distanceMeasure(void)
  {
    float duration = 0.00;             // Float type variable declaration 
    float CM = 0.00;      
    digitalWrite(trigPin, LOW);        // Trig_pin output as OV (Logic Low-Level) 
    delayMicroseconds(2);              // Delay for 2 us    
    //Send 10us High Pulse to Ultra-Sonic Sonar Sensor "trigPin" 
    digitalWrite(trigPin, HIGH);       // Trig_pin output as 5V (Logic High-Level)
    delayMicroseconds(10);             // Delay for 10 us     
    digitalWrite(trigPin, LOW);        // Trig_pin output as OV (Logic Low-Level)    
    duration = pulseIn(echoPin, HIGH); // Start counting time, upto again "echoPin" back to Logical "High-Level" and puting the "time" into a variable called "duration" 
    CM = (duration / 58.82); //Convert distance into CM.     
    return CM;
  }



void moveForward() {
  analogWrite(enA, 255);
  analogWrite(enB, 255);

  digitalWrite(in1, HIGH);
  digitalWrite(in4, LOW);

  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);

}

void moveBackward() {
  analogWrite(enA, 255);
  analogWrite(enB, 255);

  digitalWrite(in1, LOW);
  digitalWrite(in4, HIGH);

  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);

}


void hault() {
  analogWrite(enA, 255);
  analogWrite(enB, 255);

  digitalWrite(in1, LOW);
  digitalWrite(in4, LOW);

  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);

}

void turnLeft() {
  analogWrite(enA, 255);
  analogWrite(enB, 255);

  digitalWrite(in1, HIGH);
  digitalWrite(in4, HIGH);

  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);

}

void turnRight() {
  analogWrite(enA, 255);
  analogWrite(enB, 255);

  digitalWrite(in1, LOW);
  digitalWrite(in4, LOW);

  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);

}

void put_off_fire()  // 1 round dan-bam kore pani dbe
{
  hault();
  delay (200);
    
  digitalWrite(pump, LOW);
  delay(300);
  bool cholbe_ki;  // if all reading during sweep is low then loop runs once else keep running till all reading low
  do { 
    cholbe_ki = false; 
    for (us_servo_pos = 90; us_servo_pos <= 130; us_servo_pos += 1) { 
      if (digitalRead(fire)) cholbe_ki = true;
      US_servo.write(us_servo_pos); 
      delay(10);  
    }
    for (us_servo_pos = 130; us_servo_pos >= 50; us_servo_pos -= 1) { 
      if (digitalRead(fire)) cholbe_ki = true;
      US_servo.write(us_servo_pos); 
      delay(10);
    }
    for (us_servo_pos = 50; us_servo_pos <= 90; us_servo_pos += 1) { 
      if (digitalRead(fire)) cholbe_ki = true;
      US_servo.write(us_servo_pos);
      delay(10);
    }
  } while(cholbe_ki); // run till 1 whole sweep has no fire detected
  
  digitalWrite(pump,HIGH);
  US_servo.write(90);
  
}

void avoid_obstacle() {
  float leftdist, rightdist;
  hault(); //Robot Stop
  delay(100);
  US_servo.write(5);
  delay(500);
  rightdist = distanceMeasure();
  delay(100);
  US_servo.write(180);
  delay(1000);
  leftdist = distanceMeasure();
  delay(100);
  US_servo.write(90);
  delay(500);
  // compare the distances and make decision
  if(leftdist > rightdist) { 
    moveBackward();
    delay(100);
    hault();
    delay(100);
    turnLeft();
    delay(400);
    hault();
    delay(100);  
  }
  else {
    moveBackward();
    delay(100);
    hault();
    delay(100);
    turnRight();
    delay(400);
    hault();
    delay(100);             
  }
}




// void to_fire() {  using 3 digital flame sensor: right now we test if 1 analog sensor is enough
//   // need to experiment (use another threholding to estimate safe distance from fire)
//   // use image processing for fire as well
//   if (digitalRead(fs_left) ==1 && digitalRead(fs_right)==1 && analogRead(fs_front) > fs_threshold) hault();
//     else if (digitalRead(fs_front) ==0) moveForward();
//     else if (digitalRead(fs_left) ==0) turnRight();
//     else if (digitalRead(fs_right) ==0) turnLeft();
// }