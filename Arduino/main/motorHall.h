/*
 * motorHall.h
 * Motor usage via hall sensors, implement via INT0/INT1
 * Allows for *precise* movement control
 * 
 * Notes:
 * Every revolution, count ~= 190
 * 
 * Issues: PWM not done yet, specifying anything other than 90 or 100
 * may result in unwanted behaviour (not enough juice to drive ALEX)
 * 
 * To implement: set 0 as param 1 for infinite behaviour (?)
 */
 
/* Constants:
 * #define M1F 6 // Left forward pin
 * #define M1R 5 // Left reverse pin
 * #define M2F 10  // Right forward pin
 * #define M2R 11 // Right reverse pin
 */

#define COUNTS_PER_REV 190 // Number of ticks per revolution from the wheel encoder.
#define WHEEL_CIRC 20 //Wheel circumference in cm.
#define COUNTS_PER_RATURN 65 //Number of ticks per right angle turn 

static volatile unsigned long leftCount = 0; //Store the tick count from Alex's left and right encoders.
static volatile unsigned long rightCount = 0;
static volatile double leftRevs = 0; //Store the revolutions on Alex's left and right wheels
static volatile double rightRevs = 0;
static volatile unsigned long forwardDist = 0; //Forward and backward distance traveled
static volatile unsigned long reverseDist = 0;
static volatile unsigned long maxCount = 0; //max count to stop

/**
 * Setup Code for Interrupts
 */

void setupMH() {
 DDRD &= 0b11110011;
 PORTD |= 0b00001100;
 EICRA = 0b00001010; //falling edge, PIN2/3
 EIMSK = 0b00000011; //enable interrupt
}

ISR(INT0_vect) {
  leftCount = leftCount + 1;
  leftRevs = (double) leftCount / COUNTS_PER_REV;
  forwardDist = leftRevs * WHEEL_CIRC; //assumed wheels are straight
}

ISR(INT1_vect) {
  rightCount = rightCount + 1;
  rightRevs = (double) rightCount / COUNTS_PER_REV;
}

/**
 * Setup Suite Code
 * To be implemented later w. baremetal programming
 */

void setupSerial() {
  
}

void startSerial() {
  
}

void setupMotors() {
  
}

void startMotors() {
  
}

void resetGlobalsH() { //reset all values
  leftCount= 0;
  rightCount = 0;
  leftRevs = 0;
  rightRevs = 0;
  //forwardDist = 0; 
  //reverseDist = 0; 
  maxCount = 0; 
}

void clearOneCounter(int state) { //Clears one particular counter
  switch(state) {
    case 0:
      resetGlobalsH();
      break;
    case 1:
      leftCount=0;
      break;
    case 2:
      rightCount=0;
      break;
    case 3:
      leftRevs=0;
      break;
    case 4:
      rightRevs=0;
      break;
    case 5:
      forwardDist=0;
      break;
    case 6:
      reverseDist=0;
      break;
  }
}

/**
 * Functions to calculate param
 */

int pwmVal(float speed) { //convert speed to pwm format
  if (speed < 0.0) {
    Serial.println("Invalid speed, defaulting to 0.");
    return 0;
  } else if (speed > 100.0) {
    Serial.println("Exceeded speed limit, defaulting to 100");
    return 100;
  } else return (int) ((speed / 100.0) * 255.0);
}

unsigned long getDistH(float dist) { //return num of tick counts
  return (unsigned long) (COUNTS_PER_REV * (dist / WHEEL_CIRC));
}

unsigned long getAngleH(float angle) { //return num of tick counts
  return (unsigned long) ((angle / 90.0) * (float) COUNTS_PER_RATURN);
}

// Stop Alex. To replace with bare-metal code later.
void stopH() {
  analogWrite(M1F, 0);
  analogWrite(M1R, 0);
  analogWrite(M2F, 0);
  analogWrite(M2R, 0);
  resetGlobalsH();
}

/**
 * Movement Code
 * Use packet, param1 is dist/angle, param2 is speed
 * dist in cm, angle in degrees
 */
 
void forwardH(float dist, float speed) {
  int val = pwmVal(speed);
  resetGlobalsH();
  maxCount = getDistH(dist);

  while (leftCount <= maxCount && rightCount <= maxCount) {
    analogWrite(M1F, val);
    analogWrite(M2F, val);
    analogWrite(M1R, 0);
    analogWrite(M2R, 0);
  }
  stopH(); 
}

void reverseH(float dist, float speed) {
  int val = pwmVal(speed);
  resetGlobalsH();
  maxCount = getDistH(dist);
  
  while (leftCount <= maxCount && rightCount <= maxCount) {
    analogWrite(M1F, 0);
    analogWrite(M2F, 0);
    analogWrite(M1R, val);
    analogWrite(M2R, val);
  }
  stopH();
}

void leftH(float angle, float speed) {
  int val = pwmVal(speed);
  resetGlobalsH();
  maxCount = getAngleH(angle);
  
  while (leftCount <= maxCount) {
    analogWrite(M1R, val);
    analogWrite(M2F, val);
    analogWrite(M1F, 0);
    analogWrite(M2R, 0);
  }
  stopH();
}

void rightH(float angle, float speed) {
  resetGlobalsH();
  int val = pwmVal(speed);
  maxCount = getAngleH(angle);
  
  while (rightCount <= maxCount) {
    analogWrite(M2R, val);
    analogWrite(M1F, val);
    analogWrite(M1R, 0);
    analogWrite(M2F, 0);
  }
  stopH();
}

/**
 * Hall Effect Sensor Setup Code
 * Call to init sensors.
 */
void setupMotorHall() {
  resetGlobalsH();
  cli();
  setupMH();
  Serial.println("motorHall.h setup complete");
  setupSerial();
  startSerial();
  setupMotors();
  startMotors();
  //initializeState();
  sei();
}
