/*
 * motor.h
 * Regular motor code for Alex
 * Use analogWrite to assign
 * movement strictly controlled via delay
 */

/* Constants:
 * #define M1F 6 // Left forward pin
 * #define M1R 5 // Left reverse pin
 * #define M2F 10  // Right forward pin
 * #define M2R 11 // Right reverse pin
 * 
 * typedef enum dir {
 *   STOP,
 *   FORWARD,
 *   BACKWARD,
 *   LEFT,
 *   RIGHT
 * } dir;
 */


static volatile int moveSpd = 0;

double checkPercent(double percent) {
  if(percent < 0.0) return 0.0;
  else if(percent > 100.0) return 100.0;
  else return percent;
}

// Get PWM value based on percentage
int getPWMValue(double percent) {
  return (int)(percent/100.0 * 255.0); 
}

void move(double percent, int direction) {
  int val=getPWMValue(checkPercent(percent));

  // Forward and reverse values for motors 1 and 2
  int M1FVal, M1RVal, M2FVal, M2RVal;

  switch(direction) {
    case FORWARD:    
      M1FVal=0;
      M2FVal=0;
      M1RVal=val;
      M2RVal=val;  
    break;

    case BACKWARD:
      M1FVal=val;
      M2FVal=val;
      M1RVal=0;
      M2RVal=0; 
    break;

    case LEFT:
     M1FVal=val;
     M2FVal=0;
     M1RVal=0;
     M2RVal=val;
    break;
    
    case RIGHT:
      M1FVal=0;
      M2FVal=val;
      M1RVal=val;
      M2RVal=0;
    break;

    case STOP:
    default:

    M1FVal=0;
    M2FVal=0;
    M1RVal=0;
    M2RVal=0;
  }

  analogWrite(M1F, M1FVal);
  analogWrite(M2F, M2FVal);
  analogWrite(M1R, M1RVal);
  analogWrite(M2R, M2RVal);
}

//basic ALEX movement
void forward(double percent) {
  move(percent, FORWARD);
}

void backward(double percent) {
  move(percent, BACKWARD);
}

void left(double percent) {
  move(percent, LEFT);
}

void right(double percent) {
  move(percent, RIGHT);
}

void stop() {
  move(0.0, STOP);
} 

//controlled movement
void accelerate() { //only increment in 10%
  moveSpd = (moveSpd <= 100 ? moveSpd + 10 : moveSpd);
  move(moveSpd, FORWARD);
}

void deccelerate() {
  moveSpd = (moveSpd >= 10 ? moveSpd - 10 : moveSpd);
  move(moveSpd, FORWARD);
}

void slowLeft() {
  moveSpd = 50;
  move(moveSpd, LEFT);
}

void slowRight() {
  moveSpd = 50;
  move(moveSpd, RIGHT);
}

void accelerateBack() {
  moveSpd = (moveSpd <= 100 ? moveSpd + 10 : moveSpd);
  move(moveSpd, BACKWARD);
}

void deccelerateBack() {
  moveSpd = (moveSpd >= 10 ? moveSpd - 10 : moveSpd);
  move(moveSpd, BACKWARD);
}

void moveFwd() {
  for (int i = 0; i < 5; i++) {
    accelerate();
    delay(200);
  }
}

void slowFwdStop() {
  for (int i = 0; i < 5; i++) {
    deccelerate();
    delay(100);
  }
}

void moveBack(){
  for (int i = 0; i < 5; i++) accelerateBack();
}

void leftTurn() { //turn 90 degrees left
  stop();
  move(100, LEFT);
  delay(200);
  stop();
}

void rightTurn() { //turn 90 degrees right
  stop();
  move(100, RIGHT);
  delay(200);
  stop();
}

void uTurn() { //make a U turn using a left turn
  stop();
  left(100);
  delay(400);
}
