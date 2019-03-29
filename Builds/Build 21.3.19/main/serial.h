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


/******************************************************************
* Serial Communication Packet
* 12 Bytes, primitive packet
******************************************************************/
//serial communication packet
/*typedef struct { //input packet, 12 bytes
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
}*/

/******************************************************************/
/******************************************************************
* Arduino TPacket Serial Communication
* 100 Bytes, provided via project materials
******************************************************************/
//packet definition

#ifndef __CONTROL_H__
#define __CONTROL_H__

#include <stdint.h>

#define MAX_STR_LEN   32
// This packet has 1 + 1 + 2 + 32 + 16 * 4 = 100 bytes
typedef struct {
  char packetType;
  char command;
  char dummy[2]; // Padding to make up 4 bytes
  char data[MAX_STR_LEN]; // String data
  uint32_t params[16];
} TPacket;



#endif


//will have to figure this out later on
#ifndef __SERIALIZE__
#define SERIALIZE

#include <stdlib.h>

#define PACKET_SIZE 140
#define MAX_DATA_SIZE 128


int serialize(char *buffer, void *dataStructure, size_t size);
TResult deserialize(const char *buffer, int len, void *output);

#endif

TResult readPacket(TPacket *packet) { //parses in packet and determines info
    char buffer[PACKET_SIZE];
    int len;

    len = readSerial(buffer);

    if(len == 0)
      return PACKET_INCOMPLETE;
    else
      return deserialize(buffer, len, packet);
}

void sendResponse(TPacket *packet) {
  // Takes a packet, serializes it then sends it out
  // over the serial port.
  char buffer[PACKET_SIZE];
  int len;

  len = serialize(buffer, packet, sizeof(TPacket));
  writeSerial(buffer, len);
}

void sendStatus() {
  
  // Implement code to send back a packet containing key
  // information like leftTicks, rightTicks, leftRevs, rightRevs
  // forwardDist and reverseDistlen
  // Use the params array to store this information, and set the
  // packetType and command files accordingly, then use sendResponse
  // to send out the packet. See sendMessage on how to use sendResponse.
  //
  TPacket statusPacket;
  statusPacket.packetType = PACKET_TYPE_RESPONSE;
  statusPacket.command = RESP_STATUS;
  statusPacket.params[0] = leftCount;
  statusPacket.params[1] = rightCount;
  /*statusPacket.params[2] = leftRTicks;
  statusPacket.params[3] = rightRTicks;
  statusPacket.params[4] = leftFTicksTurns;
  statusPacket.params[5] = rightFTicksTurns;
  statusPacket.params[6] = leftRTicksTurns;
  statusPacket.params[7] = rightRTicksTurns;
  statusPacket.params[8] = forwardDist;
  statusPacket.params[9] = reverseDist;*/
  sendResponse(&statusPacket);
  
}

void sendMessage(const char *message) {
  // Sends text messages back to the Pi. Useful
  // for debugging.
  
  TPacket messagePacket;
  messagePacket.packetType=PACKET_TYPE_MESSAGE;
  strncpy(messagePacket.data, message, MAX_STR_LEN);
  sendResponse(&messagePacket);
}

void sendBadPacket() {
  // Tell the Pi that it sent us a packet with a bad
  // magic number.
  
  TPacket badPacket;
  badPacket.packetType = PACKET_TYPE_ERROR;
  badPacket.command = RESP_BAD_PACKET;
  sendResponse(&badPacket);
  
}

void sendBadChecksum() {
  // Tell the Pi that it sent us a packet with a bad
  // checksum.
  
  TPacket badChecksum;
  badChecksum.packetType = PACKET_TYPE_ERROR;
  badChecksum.command = RESP_BAD_CHECKSUM;
  sendResponse(&badChecksum);  
}

void sendBadCommand() {
  // Tell the Pi that we don't understand its
  // command sent to us.
  
  TPacket badCommand;
  badCommand.packetType=PACKET_TYPE_ERROR;
  badCommand.command=RESP_BAD_COMMAND;
  sendResponse(&badCommand);

}

void sendBadResponse() {
  TPacket badResponse;
  badResponse.packetType = PACKET_TYPE_ERROR;
  badResponse.command = RESP_BAD_RESPONSE;
  sendResponse(&badResponse);
}

void sendOK() {
  TPacket okPacket;
  okPacket.packetType = PACKET_TYPE_RESPONSE;
  okPacket.command = RESP_OK;
  sendResponse(&okPacket);  
}
