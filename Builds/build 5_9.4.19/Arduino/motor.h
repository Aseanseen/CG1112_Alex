/**************************************************************************************************
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
 * New Implementation, timer interrupts get PWM value for OCRx
 * 
 * To implement: set 0 as param 1 for infinite behaviour (?)
**************************************************************************************************/
 
/*=================================================================================================
  Constants:
  #define M1F 6 // Left forward pin
  #define M1R 5 // Left reverse pin
  #define M2F 10  // Right forward pin
  #define M2R 11 // Right reverse pin
  
  #define COUNTS_PER_REV 190 //Num of Ticks per Revolution
  #define WHEEL_CIRC 20 //Wheel circumference in cm.
  #define COUNTS_PER_RATURN 65 //Number of ticks per right angle turn
=================================================================================================*/
/*|Global Variable Declarations|******************************************************************/
/*static volatile unsigned long leftCount = 0; //Store encoder tick count
static volatile unsigned long rightCount = 0;
static volatile double leftRevs = 0; //Revolution count per wheel
static volatile double rightRevs = 0;
static volatile unsigned long forwardDist = 0;
static volatile unsigned long reverseDist = 0;
static volatile unsigned long maxCount = 0; //max count to stop
static volatile unsigned long speed = 0;
static volatile long distFront = 0;*/

/**************************************************************************************************
 * Setup Code for Hall Effect Sensor
**************************************************************************************************/
void setupMH() { //setup interrupts
 DDRD &= 0b11110011; //setup PIN2/3
 PORTD |= 0b00001100;
 EICRA = 0b00001010; //falling edge
 EIMSK = 0b00000011; //enable interrupts INT0/1
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

/**************************************************************************************************
 * Global Variable Reset Functions
**************************************************************************************************/
void resetGlobalsH() { //reset all values
  leftCount= 0;
  rightCount = 0;
  leftRevs = 0;
  rightRevs = 0;
  forwardDist = 0; 
  reverseDist = 0; 
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

/**************************************************************************************************
 * Parameter Calculation Functions
**************************************************************************************************/
int getPWM(float speed) { //convert speed to pwm format
  if (speed < 0.0) {
    message = "Invalid spd, default 0\n";
    sendMsgAuto();
    return 0;
  } else if (speed > 100.0) {
    message = "Exceeded spd limit, default 100\n";
    sendMsgAuto();
    return 100;
  } else return (int) ((speed / 100.0) * 255.0);
}

unsigned long getDistH(float dist) { //return num of tick counts
  return (unsigned long) (COUNTS_PER_REV * (dist / WHEEL_CIRC));
}

unsigned long getAngleH(float angle) { //return num of tick counts
  return (unsigned long) ((angle / 90.0) * (float) COUNTS_PER_RATURN);
}

/**************************************************************************************************
 * Setup Suite Code
**************************************************************************************************/
//PWM-based setup
void setupMotors() {
  DDRD |= 0b01100000; //pin5/6
  TCNT0 = 0;
  OCR0A = 0;
  OCR0B = 0;
  TIMSK0 |= 0b110;
  TCCR0B = 0b00000011;

  DDRB |= 0b00000110; //pin9/10
  TCNT1 = 0;
  OCR1A = 0;
  OCR1B = 0;
  TIMSK1 |= 0b110;
  TCCR1B = 0b00000011;
}

ISR(TIMER0_COMPA_vect) {
  OCR0A = RMOTORCOMP * getPWM(speed);
}

ISR(TIMER0_COMPB_vect) {
  OCR0B = RMOTORCOMP * getPWM(speed);
}

ISR(TIMER1_COMPA_vect) {
  OCR1A = LMOTORCOMP * getPWM(speed);
}

ISR(TIMER1_COMPB_vect) {
  OCR1B = LMOTORCOMP * getPWM(speed);
}

void initializeState() {
  resetGlobalsH;
}

/**************************************************************************************************
 * Movement Code
 * Configured for packet currently
 * param1 is dist/angle, param2 is speed 
 * dist in cm, angle in degrees
**************************************************************************************************/
/*analogWrite version*/
/*void stopH() {
  analogWrite(M1F, 0);
  analogWrite(M1R, 0);
  analogWrite(M2F, 0);
  analogWrite(M2R, 0);
  resetGlobalsH();
}

void forwardH(float dist, float speed) {
  int val = getPWM(speed);
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
  int val = getPWM(speed);
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
  int val = getPWM(speed);
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
  int val = getPWM(speed);
  maxCount = getAngleH(angle);
  
  while (rightCount <= maxCount) {
    analogWrite(M2R, val);
    analogWrite(M1F, val);
    analogWrite(M1R, 0);
    analogWrite(M2F, 0);
  }
  stopH();
}*/

/*|bare-metal version|****************************************************************************/
void stopH() {
  PORTD |= 0b01100000;
  PORTB |= 0b00000110;
  OCR0A = 0;
  OCR0B = 0;
  OCR1A = 0;
  OCR1B = 0;
  resetGlobalsH();
}

void forwardH(float dist, float speed) {
  resetGlobalsH();
  speed = speed;
  maxCount = getDistH(dist);
  distFront = getDistUS();
  
  TCCR0A = 0b11000001;
  TCCR1A = 0b00110001;
  while (leftCount <= maxCount && rightCount <= maxCount) {
    distFront = getDistUS();
    if (distFront <= 10 && distFront != 0) break;
    PORTD &= 0b11011111;
    PORTB &= 0b11111101;

    //course-correction
    if (leftCount - rightCount >= 50) {
      OCR1A = (LMOTORCOMP * getPWM(speed)) * COURSECOMP;
      OCR1B = (LMOTORCOMP * getPWM(speed)) * COURSECOMP;
    } else {
      OCR1A = (LMOTORCOMP * getPWM(speed));
      OCR1B = (LMOTORCOMP * getPWM(speed));
    }

    if (rightCount - leftCount >= 50) {
      OCR0A = (RMOTORCOMP * getPWM(speed)) * COURSECOMP;
      OCR0B = (RMOTORCOMP * getPWM(speed)) * COURSECOMP;
    } else {
      OCR0A = (RMOTORCOMP * getPWM(speed));
      OCR0B = (RMOTORCOMP * getPWM(speed));
    }
    
    if (maxCount - leftCount <= 150 || maxCount - rightCount <= 150)
      speed = max(speed - 10, 30);
  }
  stopH(); 
}

void reverseH(float dist, float speed) {
  resetGlobalsH();
  speed = speed;
  maxCount = getDistH(dist);

  TCCR0A = 0b00110001;
  TCCR1A = 0b11000001;
  while (leftCount <= maxCount && rightCount <= maxCount) {
    PORTD &= 0b10111111;
    PORTB &= 0b11111011;

    //course-correction
    if (leftCount - rightCount >= 50) {
      OCR1A = (LMOTORCOMP * getPWM(speed)) * COURSECOMP;
      OCR1B = (LMOTORCOMP * getPWM(speed)) * COURSECOMP;
    } else {
      OCR1A = (LMOTORCOMP * getPWM(speed));
      OCR1B = (LMOTORCOMP * getPWM(speed));
    }

    if (rightCount - leftCount >= 50) {
      OCR0A = (RMOTORCOMP * getPWM(speed)) * COURSECOMP;
      OCR0B = (RMOTORCOMP * getPWM(speed)) * COURSECOMP;
    } else {
      OCR0A = (RMOTORCOMP * getPWM(speed));
      OCR0B = (RMOTORCOMP * getPWM(speed));
    }
    
    if (maxCount - leftCount <= 150 || maxCount - rightCount <= 150)
      speed = max(speed - 10, 30);
  }
  stopH(); 
}

void leftH(float angle, float speed) {
  resetGlobalsH();
  speed = speed;
  maxCount = getAngleH(angle);
  distFront = getDistUS();

  TCCR1A = 0b00110001;
  TCCR0A = 0b00110001;
  while (leftCount <= maxCount && rightCount <= maxCount) {
    distFront = getDistUS();
    PORTD &= 0b10111111;
    PORTB &= 0b11111101;
    if (maxCount - leftCount <= 150 || maxCount - rightCount <= 150)
      speed = max(speed - 10, 30);
  }
  stopH(); 
}

void rightH(float angle, float speed) {
  resetGlobalsH();
  speed = speed;
  maxCount = getAngleH(angle);
  distFront = getDistUS();

  TCCR0A = 0b11000001;
  TCCR1A = 0b11000001;
  while (leftCount <= maxCount && rightCount <= maxCount) {
    distFront = getDistUS();
    PORTD &= 0b11011111;
    PORTB &= 0b11111011;
    if (maxCount - leftCount <= 150 || maxCount - rightCount <= 150)
      speed = max(speed - 10, 30);
  }
  stopH(); 
}

//slowly approach obj in front
void approachH()  {
  resetGlobalsH();
  speed = 50;
  distFront = getDistUS();
  
  TCCR0A = 0b11000001;
  TCCR1A = 0b00110001;
  while (distFront >= 3) {
    distFront = getDistUS();
    PORTD &= 0b11011111;
    PORTB &= 0b11111101;

    //course-correction
    if (leftCount - rightCount >= 50) {
      OCR1A = (LMOTORCOMP * getPWM(speed)) * COURSECOMP;
      OCR1B = (LMOTORCOMP * getPWM(speed)) * COURSECOMP;
    } else {
      OCR1A = (LMOTORCOMP * getPWM(speed));
      OCR1B = (LMOTORCOMP * getPWM(speed));
    }

    if (rightCount - leftCount >= 50) {
      OCR0A = (RMOTORCOMP * getPWM(speed)) * COURSECOMP;
      OCR0B = (RMOTORCOMP * getPWM(speed)) * COURSECOMP;
    } else {
      OCR0A = (RMOTORCOMP * getPWM(speed));
      OCR0B = (RMOTORCOMP * getPWM(speed));
    }
    if (distFront <= 20 && distFront >= 9) speed = 40;
    if (distFront <=  8) speed = max(speed - 10, 20);
  }
  
  speed = 0;
  stopH(); 
  
}

/**************************************************************************************************
 * Hall Effect Sensor Setup Code
 * Call to init sensors.
 * Calls various setup functions
**************************************************************************************************/
void setupMotorHall() {
  resetGlobalsH();
  cli();
  setupMH();
  setupMotors(); //for bare-metal version
  initializeState();
  sei();
}
