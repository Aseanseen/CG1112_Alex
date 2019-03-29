/**************************************************************************************************
 * serial.h
 * Setup lib for serial communication
**************************************************************************************************/

void setupSerial() { 
  Serial.begin(57600); //replace with bare-metal later on
}

void startSerial() { //replace with bare-metal later on
} 

void waitForHello() { //initial comms setup routine
  int exit=0;

  while(!exit) {
    TPacket hello;
    TResult result;
    
    do {
      result = readPacket(&hello);
    } while (result == PACKET_INCOMPLETE);

    if(result == PACKET_OK) {
      if(hello.packetType == PACKET_TYPE_HELLO) {
        sendOK();
        exit=1;
      }
      else
        sendBadResponse();
    }
    else
      if(result == PACKET_BAD) {
        sendBadPacket();
      }
      else
        if(result == PACKET_CHECKSUM_BAD)
          sendBadChecksum();
  } // !exit
}
