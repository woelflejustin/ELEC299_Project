/*
 * Header File to declare the motor structure along with all of its functions. Used in ELEC_299_Lab_2.
 */

/*
 * The motor structure is used for the driving motors of the robot. The entries are
 * defined as follows:
 * speedPin, directionPin, encoderPin: Arduino pins to control speed, direction, and ecoders
 * motorSpeed: PWM value written to an analog pin to control the speed of the motor
 * motorDirection: High or low value to decide direction; high == forward, low == backward
 * rps: The rotation speed of the wheels as calculated by the encoders in rot/s
 * encoderCount: The count of the motor's optical encoders
 * *encoderCounter(): The encoder counting function to be attached to each motor as the ISR
 */

typedef struct motor {
  int speedPin;
  int directionPin;
  int encoderPin;
  int motorSpeed;
  int motorDirection;
  float rps;
  int encoderCount;
  void (*encoderCounter)();
} motor;

/*
 * Initializing function for a motor. The parameters are as follows:
 * sPin, dPin, ePin: Integers representing the pins for speed, direction, and the encoder
 * *eCounter(): The ISR to be attached to the returned motor pointer
 */

motor* NewMotor(int sPin, int dPin, int ePin, void (*eCounter)()) {
  motor *m = (motor*)malloc(sizeof(motor));
  if(!m)
    return NULL;
  m->speedPin = sPin;
  m->directionPin = dPin;
  m->encoderPin = ePin;
  m->encoderCount = 0;
  m->encoderCounter = eCounter;
  return m;
}


/*
 * Sets the pins of the motor m to the specified state.
 */
 
void SetPinModes(motor *m) {
  pinMode(m->speedPin, OUTPUT);
  pinMode(m->directionPin, OUTPUT);
  pinMode(m->encoderPin, INPUT);
}

/*
 * Writes the motor to a desired state. The parameters are as follows:
 * *m: The pointer to the motor to be changed
 * s: The PWM speed to be written to the motor
 * d: The direction to be written to the motor
 * Along with writing the desired states to the motor, the function also
 * saves the states it writes to the motor so they can be accessed quickly
 * by other routines.
 */
 
void WriteMotor(motor *m, int s, int d) {
  m->motorSpeed = s;
  m->motorDirection = d;
  analogWrite(m->speedPin, s);
  digitalWrite(m->directionPin, d);
}

/*
 * Resetszh encoderCount of the specified motor to 0.
 */

void ResetEncoder(motor *m) {
  m->encoderCount = 0;
}
