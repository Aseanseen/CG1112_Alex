#include "motor.h"
#include "motorHall.h"

void setup() {
  // put your setup code here, to run once:
  setupMotorHall();
  
  Serial.println("Hall effect sensors intialized");

  Serial.println("All systems nominal.");
  Serial.begin(57600);
}

static volatile int com = 0;

typedef struct { //key-value state pair
  int state;
  int val;
} command;

void loop() {
  //print status
  Serial.print("left: ");
  Serial.print(leftCount);
  Serial.print("   right: ");
  Serial.println(rightCount);

  delay(200);
  
  //serial inputs
  String serialInput; //grab serial input
  if (Serial.available()) {
    serialInput = Serial.readString();
    Serial.print("Input: ");
    Serial.println(serialInput);
  }

  //decode command
  if (serialInput == "f") com = 1;
  else if (serialInput == "l") com = 2;
  else if (serialInput == "r") com = 3;
  else if (serialInput == "b") com = 4;
  else com = 0;

  if (com != 0) Serial.println(com);

  switch(com) {
    case 0: //idle
      break;
    case 1:
      Serial.println("Move Forward");
      //moveFwd();
      forwardH(10, 90);
      delay(500);
      stop();
      break;
    case 2:
      Serial.println("Left Turn");
      leftTurn();
      delay(500);
      break;
    case 3:
      Serial.println("Right Turn");
      rightTurn();
      delay(500);
      break;
    case 4:
      Serial.println("Move Backward");
      reverseH(10, 90);
      delay(500);
      stop();
      break;
  }
  serialInput = "";

  //note: forward() is actually backwards
  //leftTurn();
  //delay(1000);

}
