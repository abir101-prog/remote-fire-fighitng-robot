#include <Arduino.h>
#include <Servo.h>
#include <SoftwareSerial.h>

#define rxPin 12
#define txPin 13
// SoftwareSerial esp_serial =  SoftwareSerial(rxPin, txPin);
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
int check();

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
// int fs_front = A4;

// internal variables
// US servo
int us_servo_pos = 0;

//fire detection
int fire = A5; // digital pin of flame sensor 
const int fs_threshold = 800;  // for analog input sensor

int last_turn = 0;  // 0: no data: 1: right, -1:left


void setup() {
  Serial.begin(115200);
  // esp_serial.begin(115200);  // might have to change
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
  // pinMode(fs_front, INPUT);
  pinMode(fire, INPUT);
  digitalWrite(pump, LOW); // maybe inverter
  // servo initial position
  US_servo.attach(servo_pin);
  US_servo.write(90); 
  // moveForward();
  // delay(200);
  // moveBackward();
  // delay(200);
  // turnLeft();
  // delay(100);
  // turnRight();
  // delay(100);
  // hault();
  delay(5000);

}

// this 2 data(koto angle ghora lagbe(cw) or olpo kondike ghora lagbe (if steps=0 dir 0 or 2))
// if step=0 and dir > 2 data is invalid
int cam_servo_angle = 0;
// int direction = 5; // initially eta matter korena cz 5 (1 2 3) (l f r)
bool been_ordered = false;  // ordered to move by esp?

void loop() {
  int chk = check();
  while (chk==1) {delay(500); chk=check();} // flame sensor jotokhon detect korbe totokhon etai korbe
  // if (chk==2) {put_off_fire(); }  // temporarily turned off for test
  delay(1000);
  last_turn = 0;

  while (Serial.available()) {
    //Serial.println(Serial.available()); 
    cam_servo_angle = Serial.read(); //Serial.println(cam_servo_angle);
    if (cam_servo_angle==1 || cam_servo_angle % 10 == 0) { 
      been_ordered =true; 
      Serial.println(cam_servo_angle);
      Serial.end(); 
      Serial.begin(115200); 
      break; }
  }
  // // // Serial.println("lol");
  // if (!been_ordered) {
  //   hault();
  //   delay(500);
  // } else {
  //   // fire anywhre?
  //   bool located = locate_fire();  //detect and turn the robot in that direction

  //   // if (located & (distanceMeasure() < 10)) put_off_fire();

  //   // manuever
  //   float distance = distanceMeasure();
  //   if (distance < 10) avoid_obstacle();
  //   // else {
  //     int rel_angle = cam_servo_angle - 90;  // relative wrt middle axis
  //     // Serial.println(rel_angle);
  //     if (abs(rel_angle) <= 10) {
  //        moveForward(); delay(100); hault(); 
  //       //  Serial.println("Moving forward");

  //     } else if (rel_angle > 0) { // cw
  //         turnLeft(); delay(60); hault();
  //         // Serial.println("Moving Left");

  //     } else {
  //         turnRight(); delay(60); hault();
  //         // Serial.println("Moving Right");
  //     }
  //   // }
  // }
  // been_ordered = false;

}

int check() {
  US_servo.write(40);
  delay(400);
  int right = digitalRead(fire); //analogRead(fs_front);
  US_servo.write(90);
  delay(400);
  int mid = digitalRead(fire); //analogRead(fs_front);
  US_servo.write(140);
  delay(400);
  int left = digitalRead(fire); //analogRead(fs_front);
  US_servo.write(90);
  delay(400);
  // Serial.println(right);
  // Serial.println(left);
  // Serial.println(mid);
  if (left==0 && right==0 && mid==0) return 2;  // agun kache
  else if (left==0 &&  right==1) {
    if (last_turn==1) return 2;
    turnLeft(); delay(200); hault(); return 1;
  }
  else if (left==1 && right==0) {
    if (last_turn==-1) return 2;
    turnRight(); delay(200); hault(); return 1;
  }
  else if (mid==0) {moveForward(); delay(200); hault(); return 1;}
  return 0;  // paynai
}


// bool fire_detected() {
  // int sensor_data = analogRead(fs_front);
  // Serial.println(sensor_data);
  // if (sensor_data < fs_threshold) fire = true;
  // else fire = false;
  // return fire;
// }

bool locate_fire() {  // angle of view might need to be extended
  if (digitalRead(fire)) {
    moveForward();
    delay(70);
    hault();
    delay(200);
    return true;
  }
  for (us_servo_pos = 90; us_servo_pos <= 130; us_servo_pos += 5) { 
    US_servo.write(us_servo_pos); 
    delay(50); 
    if (digitalRead(fire)) {
      turnLeft();
      delay(60);
      hault();
      US_servo.write(90);
      return true;
    }
  }
  for (us_servo_pos = 130; us_servo_pos >= 50; us_servo_pos -= 5) { 
    US_servo.write(us_servo_pos); 
    delay(50);
    if (digitalRead(fire)) {
      turnRight();
      delay(60);
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

void put_off_fire() { // 1 round dan-bam kore pani dbe
// { Serial.println("Pump should run");
  hault();
  delay(200);
    
  digitalWrite(pump, HIGH);
  delay(300);
  bool cholbe_ki;  // if all reading during sweep is low then loop runs once else keep running till all reading low
  do { 
    cholbe_ki = false; 
    for (us_servo_pos = 90; us_servo_pos <= 130; us_servo_pos += 1) { 
      if (!digitalRead(fire)) cholbe_ki = true;
      US_servo.write(us_servo_pos); 
      delay(10);  
    }
    for (us_servo_pos = 130; us_servo_pos >= 50; us_servo_pos -= 1) { 
      if (!digitalRead(fire)) cholbe_ki = true;
      US_servo.write(us_servo_pos); 
      delay(10);
    }
    for (us_servo_pos = 50; us_servo_pos <= 90; us_servo_pos += 1) { 
      if (!digitalRead(fire)) cholbe_ki = true;
      US_servo.write(us_servo_pos);
      delay(10);
    }
  } while(cholbe_ki); // run till 1 whole sweep has no fire detected
  
  digitalWrite(pump,LOW);
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