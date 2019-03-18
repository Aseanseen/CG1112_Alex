/**
 * serial.h
 * Handles serial communication between Pi and Arduino
 * 
 * Stuff to implement:
 * sendPacket()
 * 
 * sendStatus()
 * sendOK()
 */

/**
 * Packet Communication Table:
 * 0 - Error, null packet
 * 2 - Command executed, flushed state
 * l - Motor Left
 * r - Motor Right
 * f - Motor Forward
 * b - Motor Back
 * h - Help Menu
 */

//serial communication packet
typedef struct { //input packet, 12 bytes
  char cmd; //corresponds to switch statement lookup
  char cmdBuffer[3];
  int32_t param1;
  int32_t param2;
} packet;

packet getPacket() { //parse buffer
  packet newPacket;
  if (Serial.available()) {
    String buffer1, buffer2; //buffer1 - string, buffer2 - temp
    int counter = 2;
    buffer1 = Serial.readString();
    while (counter < buffer1.length() && buffer1[counter] != ' ') {
      buffer2 += buffer1[counter];
      counter++;
    }
    newPacket.param1 = buffer2.toInt();
    buffer2 = "";
    counter++;
    
    while (counter < buffer1.length() && buffer1[counter] != ' ') {
      buffer2 += buffer1[counter];
      counter++;
    }
    
    newPacket.param2 = buffer2.toInt();
    newPacket.cmd = buffer1[0];
  } else newPacket.cmd = '0';
  return newPacket;
}

packet flushPacket(packet inputPacket) { //flush packet once executed
  inputPacket.cmd = "2";
  inputPacket.param1 = 0;
  inputPacket.param2 = 0;
  return inputPacket;
}
