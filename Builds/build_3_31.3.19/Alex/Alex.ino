/**************************************************************************************************
 * Alex Arduino Code
**************************************************************************************************/
#include <serialize.h>

#include "constants.h"
#include "ultrasonic.h"
#include "motor.h" //hall effect motor control
//#include "motorA.h" //analogue motor control
#include "packet.h"
#include "serial.h"

void setup() {
  cli();
  setupMotorHall();
  Serial.println("Hall effect sensors intialized");
  setupSerial();
  startSerial();
  Serial.println("Serial communications initialized");
  setupUS();
  Serial.println("Ultrasonic sensors intialized");
  sei();
  Serial.println("All systems intialized.");
}

void loop() {
  TPacket recvPacket; //incoming packet
  TResult result = readPacket(&recvPacket);
  
  if(result == PACKET_OK) {
    handlePacket(&recvPacket);
  } else {
    if(result == PACKET_BAD) {
      sendBadPacket();
    } else if(result == PACKET_CHECKSUM_BAD) {
        sendBadChecksum();
    } 
  }
}
