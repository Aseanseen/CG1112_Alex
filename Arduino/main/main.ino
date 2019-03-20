#include "constants.h"
#include "motor.h"
#include "motorHall.h"
#include "serial.h"

#include <avr/io.h>
#include <avr/interrupt.h>

void setup() {
  Serial.begin(57600);
  setupMotorHall();
  
  Serial.println("Hall effect sensors intialized");

  Serial.println("All systems nominal.");
}

static volatile int statusCounter = 0;

static volatile int com = 0;
static String serialInput;
static packet inputPacket;
static bool isExecuting;

static volatile float moveParam1; //magnitude of movement parameter
static volatile float moveParam2; //pwm

void loop() {
  //print status every 20000 cycles
  statusCounter++;
  if (statusCounter == 20000) {
    Serial.print("left: ");
    Serial.print(leftCount);
    Serial.print("   right: ");
    Serial.println(rightCount);
    statusCounter = 0;
  }

  //fetch serial input (packet)
  
  //fetch serial input (packet from serial monitor)
  if (Serial.available()) {
    inputPacket = getPacket();  
    Serial.println(inputPacket.cmd);
    Serial.println(inputPacket.param1);
    Serial.println(inputPacket.param2);
    //isExecuting = true;
  }

  //fetch serial input (non-packet)
  /*if (Serial.available()) {
    serialInput = Serial.readString();
    Serial.print("Input: ");
    Serial.println(serialInput);
    isExecuting = true;
  }*/

  //send acknowledgement packet (broken)
  /*if (inputPacket.cmd = '2' && isExecuting) {
    Serial.println("Command executed");
    isExecuting = false;
    com = 0;
  }*/

  //decode command
  if (inputPacket.cmd == 'f') com = 1;
  else if (inputPacket.cmd == 'l') com = 2;
  else if (inputPacket.cmd == 'r') com = 3;
  else if (inputPacket.cmd == 'b') com = 4;
  else if (inputPacket.cmd == 'c') com = 100;
  else com = 0;

  switch(com) {
    case 0: //idle
      break;
    case 1:
      Serial.println("Move Forward");
      forwardH(inputPacket.param1, inputPacket.param2);
      //delay(500);
      stop();
      break;
    case 2:
      Serial.println("Left Turn");
      //leftTurn();
      leftH(inputPacket.param1, inputPacket.param2);
      //delay(500);
      break;
    case 3:
      Serial.println("Right Turn");
      //rightTurn();
      rightH(inputPacket.param1, inputPacket.param2);
      //delay(500);
      break;
    case 4:
      Serial.println("Move Backward");
      reverseH(inputPacket.param1, inputPacket.param2);
      //delay(500);
      stop();
      break;
    case 100: //flush left / right count
      leftCount = 0;
      rightCount = 0;
      break;
  }
  
  inputPacket = flushPacket(inputPacket); //flush after execution
  

}
