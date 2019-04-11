/**************************************************************************************************
 * Alex Arduino Code
**************************************************************************************************/
#include <stdint.h>
#include <serialize.h>

#include "constants.h"
#include "ultrasonic.h"
#include "motor.h" //hall effect motor control
//#include "motorA.h" //analogue motor control
#include "packet.h"
#include "serial.h"

//uncomment out Serial.println msg if debugging, else it interferes w. program
void setup() {
  cli();
  setupMotorHall();
  setupSerial();
  startSerial();
  setupUS();
  sei();
  waitForHello();
}

void loop() {
  TPacket recvPacket; //incoming packet
  TResult result = readPacket(&recvPacket);
  
  if(result == PACKET_OK) {
    handlePacket(&recvPacket);
    if (distFront <= 10) sendTooClose();
  } else {
    if(result == PACKET_BAD) {
      sendBadPacket();
    } else if(result == PACKET_CHECKSUM_BAD) {
        sendBadChecksum();
    } 
  }
}
