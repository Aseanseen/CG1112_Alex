/*
 * motorHall.h
 * Motor usage via hall sensors
 * 
 * Notes:
 * Every revolution, count ~= 190
 */
 
/* Constants:
 * #define M1F 5 // Left forward pin
 * #define M1R 6  // Left reverse pin
 * #define M2F 10  // Right forward pin
 * #define M2R 11 // Right reverse pin
 */

#define COUNTS_PER_REV 190 // Number of ticks per revolution from the wheel encoder.
#define WHEEL_CIRC 20 //Wheel circumference in cm.

static volatile unsigned long leftCount = 0; //Store the tick count from Alex's left and right encoders.
static volatile unsigned long rightCount = 0;
static volatile double leftRevs = 0; //Store the revolutions on Alex's left and right wheels
static volatile double rightRevs = 0;
static volatile unsigned long forwardDist = 0; //Forward and backward distance traveled
static volatile unsigned long reverseDist = 0;
static volatile unsigned long maxCount = 0; //max count to stop

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

// Stop Alex. To replace with bare-metal code later.
void stopH() {
  analogWrite(M1F, 0);
  analogWrite(M1R, 0);
  analogWrite(M2F, 0);
  analogWrite(M2R, 0);
  resetGlobalsH();
}

// Move Alex forward "dist" cm at speed "speed".
// "speed" is expressed as a percentage. E.g. 50 is
// move forward at half speed.
// Specifying a distance of 0 means Alex will
// continue moving forward indefinitely.
void forwardH(float dist, float speed) {
  int val = pwmVal(speed);
  resetGlobalsH();
  maxCount = getDistH(dist);
  Serial.println(maxCount);


  while (leftCount <= maxCount && rightCount <= maxCount) {
      
    analogWrite(M1F, val);
    analogWrite(M2F, val);
    analogWrite(M1R,0);
    analogWrite(M2R, 0);
  }
  stopH();
  
}

// Reverse Alex "dist" cm at speed "speed".
// "speed" is expressed as a percentage. E.g. 50 is
// reverse at half speed.
// Specifying a distance of 0 means Alex will
// continue reversing indefinitely.
void reverseH(float dist, float speed) {
  int val = pwmVal(speed);
  resetGlobalsH();
  
  analogWrite(M1R, val);
  analogWrite(M2R, val);
  analogWrite(M2F, 0);
  analogWrite(M2F, 0);

  while (leftCount <= maxCount && rightCount <= maxCount);
  stopH();
}

// Turn Alex left "ang" degrees at speed "speed".
// "speed" is expressed as a percentage. E.g. 50 is
// turn left at half speed.
// Specifying an angle of 0 degrees will cause Alex to
// turn left indefinitely.
void leftH(float ang, float speed) {
  int val = pwmVal(speed);
  resetGlobalsH();

  analogWrite(M1R, val);
  analogWrite(M2F, val);
  analogWrite(M1F, 0);
  analogWrite(M2R, 0);
}

// Turn Alex right "ang" degrees at speed "speed".
// "speed" is expressed as a percentage. E.g. 50 is
// turn left at half speed.
// Specifying an angle of 0 degrees will cause Alex to
// turn right indefinitely.
void rightH(float ang, float speed) {
  resetGlobalsH();
  int val = pwmVal(speed);

  analogWrite(M2R, val);
  analogWrite(M1F, val);
  analogWrite(M1R, 0);
  analogWrite(M2F, 0);
}

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
