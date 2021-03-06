
#include <Servo.h>
#include<time.h>
#include <Arduino_LSM6DS3.h>
Servo myservo1, myservo2, myservo3;

#define M1 7   //defined pins
#define M2 4
#define E1 6
#define E2 5
#define LENC 2
#define RENC 3
#define LBUM 11
#define RBUM 13
#define rsensor 0 // Left Sensor on Analog Pin 2
#define lsensor 1 // Right Sensor on Analog Pin 1
#define msensor 3 // Middle Sensor on Analog Pin 0

const int whitelevl = 600; // reading level is white if <600
const int blacklevl = 850; // reading level is black if >850

int counterR = 0;// first group of counters for the encoders. This group is used to determine actual speed of robot. Gets rest constantly
int counterL = 0;

int countR = 0;// second group of counters for the encoders. This group is used for positiioning along path. 
int countL = 0;

float k;// factor of difference in wheel speed
float motor_inputL = 210; // predetermined values input into the motor
float motor_inputR = 210;
long ASL; // Actual speeds of left and right wheel
long ASR;

unsigned long time1;
int zThreshold = 5; // Untested number. Would run test before selecting threshold value.
float x,y,z; // IMU acceleration regions




void setup() {
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(LBUM, INPUT);
  pinMode(RBUM, INPUT);
  pinMode(E1, OUTPUT);
  pinMode(E2, OUTPUT);
  pinMode(RENC, INPUT);
  pinMode(LENC, INPUT);
  myservo1.attach(11);
  myservo2.attach(10);
  myservo3.attach(9);

  myservo1.write(71);
  myservo2.write(180);
  interrupts();
  attachInterrupt(digitalPinToInterrupt(2), countEncoderL, CHANGE);
  attachInterrupt(digitalPinToInterrupt(3), countEncoderR, CHANGE);
  Serial.begin(9600);

  time1 = millis();
  while (!Serial);

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");

    while (1);
  }

}

void loop() {
  while (checkLine() == true || checkIMU() == true) {
    path();
  }
  reportBase(); // Code completed by another group
}

void path() { // method of covering the whole testing area
  Serial.println(counterR);
  forward();
  if ( countR <= 120) // First Straight line of 150cm
  {
    forward();
    while (countR <= 120) { // while statements matching the if statments 
      //are used to be constantly checking for home base and the speed of the robot
      checks();
    }
  }
  else if ( countR > 120 && countR <= 192) //Second straight line of 90cm
  {
    turnRight();
    forward();

    while (countR > 120 && countR <= 192) {
      checks();
    }
  }
  else if (countR > 192 && countR < 312) //Third Straight line of 150cm
  {
    turnRight();
    forward();

    while (countR > 192 && countR < 312) {
      checks();
    }
  }
  else if (countR > 312 && countR < 348) // Fourth Straight line of 45cm
  {
    turnRight();
    forward();

    while (countR > 312 && countR < 348) {
      checks();

    }
  } else if (countR > 348 && countR < 432) // Fifth straight line of 105cm
  {
    turnRight();
    forward();

    while (countR > 348 && countR < 432) {
      checks();
    }
  }

}

void checks() { // blanket method to call other methods that are constantly repeated.
  checkIMU();
  bumperCheck();
  checkSpeed();
  checkLine();
}

bool checkIMU() {
  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);

    Serial.print(x);
    Serial.print('\t');
    Serial.print(y);
    Serial.print('\t');
    Serial.println(z); \

    if (z >= zThreshold) {
      return false;
    } 
    }else {
      return true;
  }
}
void checkSpeed() {// This methods keeps the robot driving forward 
  //by having the wheels turn at the same speed.
  if (counterR >= 16) {
    ASR = (long)((millis() - time1));
    Serial.println(ASR);
    Serial.println(time1);
    Serial.println(millis());
    counterR = 0;
  }
  if (counterL >= 16) {
    ASL = (long)((millis() - time1));
    Serial.println(ASL);
    counterL = 0;
  }
  k = ASL / ASR;
  if (k > 1) {
    motor_inputR = motor_inputR + 1;
  } else if (k < 1) {
    motor_inputR = motor_inputR - 1;
  } else {
    Serial.println("Done");
  }

  digitalWrite(M1, HIGH);
  digitalWrite(M2, HIGH);
  analogWrite(E1, motor_inputR);
  analogWrite(E2, motor_inputL);

}
bool checkLine() { // needs to go until all three sensors are black


  // Case 1: All sensors are reading white. Drive forward!
  //Case 2 : One Sensor is reading black.
  if (readQD(lsensor) > blacklevl || readQD(msensor) > blacklevl || readQD(rsensor) > blacklevl) {


    // Case 2 : Left sensor and middle sensor are reading white and right sensor is reading black. Turn right!
    if (readQD(lsensor) < whitelevl && readQD(msensor) < whitelevl && readQD(rsensor) > blacklevl) {
      turnRight();
      forward();
    }

    // Case 3 : Left sensor is reading white, middle sensor and right sensor are reading black. Turn right!
    else if (readQD(lsensor) < whitelevl && readQD(msensor) > blacklevl && readQD(rsensor) > blacklevl)
    {
      turnRight();
      forward();
    }

    // Case 4 : Left sensor is reading black, middle sensor and right sensor are reading white. Turn left!
    else if (readQD(lsensor) > blacklevl && readQD(msensor) < whitelevl && readQD(rsensor) < whitelevl)
    {
      turnLeft();
      forward();
    }
    // Case 5 : Left sensor and middle sensor are reading black and right sensor is reading white. Turn left!
    else if (readQD(lsensor) > blacklevl && readQD(msensor) > blacklevl && readQD(rsensor) < whitelevl)
    {
      turnLeft();
      forward();
    }
  }
  if (readQD(lsensor) > blacklevl && readQD(msensor) > blacklevl && readQD(rsensor) > blacklevl) {
    //break loop and report flag. Code completed by another group.
    forward();
    return  false;

  } else {
    return  true;
  }
}


void bumperCheck() {// Used if robot over extents path and hits wall
  if (digitalRead(LBUM) == LOW && digitalRead(RBUM) == LOW){
      backward();
      delay(1000);
      turnRight();
    }
  
  else if (digitalRead(LBUM) == LOW) {
    Serial.print("LBUM");
    Serial.println(digitalRead(LBUM));
    turnRight();
  }

  else if (digitalRead(RBUM) == LOW) {
    Serial.print("RBUM");
    Serial.println(digitalRead(RBUM));
    turnLeft();
  }
}
void forward() {// forward drive
  analogWrite (E1,  motor_inputR);
  digitalWrite(M1, HIGH);
  analogWrite (E2, motor_inputL);
  digitalWrite(M2, HIGH);

}void backward()
{
   digitalWrite(M1, LOW);
   digitalWrite(M2, LOW);
   analogWrite(E1, 128);
   analogWrite(E2, 128);
}
void turnRight() {// right turn
  analogWrite (E1, 210);
  digitalWrite(M1, LOW);
  analogWrite (E2, 210);
  digitalWrite(M2, HIGH);
  delay(1000);
}

void turnLeft() {// left trurn
  analogWrite (E1, 210);
  digitalWrite(M1, HIGH);
  analogWrite (E2, 210);
  digitalWrite(M2, LOW);
  delay(1000);
}


int readQD(int x) {// reads sensor
  int val = analogRead(x);
  return val;
}

void countEncoderR() {

  counterR++;

}
void countEncoderL() {
  counterL++;
}
void reportBase() {
  //other group is to complete rthis section.

}

