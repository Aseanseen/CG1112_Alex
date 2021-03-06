/**************************************************************************************************
 * packet.h
 * Allows for reading of TPackets
 * 
**************************************************************************************************/
//basic serial port reading and sending
int readSerial(char *buffer) {
  int count=0;

  while(Serial.available()) //get from the serial port
    buffer[count++] = Serial.read();

  return count;
}

void writeSerial(const char *buffer, int len) {
  Serial.write(buffer, len); //replace with bare-metal later on
}

TResult readPacket(TPacket *packet) { //reads in data and deserializes it into packet
    char buffer[PACKET_SIZE];
    int len;

    len = readSerial(buffer);

    if(len == 0) return PACKET_INCOMPLETE;
    else return deserialize(buffer, len, packet);
}

void sendResponse(TPacket *packet) { //takes packet, serializes it and sends over
  char buffer[PACKET_SIZE];
  int len;

  len = serialize(buffer, packet, sizeof(TPacket));
  writeSerial(buffer, len);
}


void sendStatus() {
  TPacket statusPacket;
  statusPacket.packetType = PACKET_TYPE_RESPONSE;
  statusPacket.command = RESP_STATUS;
  statusPacket.params[0] = leftCount;  
  statusPacket.params[1] = rightCount;  
  statusPacket.params[2] = leftRevs;  
  statusPacket.params[3] = rightRevs;  
  statusPacket.params[4] = forwardDist;  
  statusPacket.params[5] = reverseDist;  
  statusPacket.params[6] = 0;  
  statusPacket.params[7] = 0;  
  statusPacket.params[8] = 0;  
  statusPacket.params[9] = 0;  
  sendResponse(&statusPacket);
}

void sendMessage(const char *message) { //send text to PI
  TPacket messagePacket;
  messagePacket.packetType=PACKET_TYPE_MESSAGE;
  strncpy(messagePacket.data, message, MAX_STR_LEN);
  sendResponse(&messagePacket);
}

void sendBadPacket() { //bad input  
  TPacket badPacket;
  badPacket.packetType = PACKET_TYPE_ERROR;
  badPacket.command = RESP_BAD_PACKET;
  sendResponse(&badPacket);
}

void sendBadChecksum() { //bad checksum
  TPacket badChecksum;
  badChecksum.packetType = PACKET_TYPE_ERROR;
  badChecksum.command = RESP_BAD_CHECKSUM;
  sendResponse(&badChecksum);  
}

void sendBadCommand() { //bad command
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

void sendOK() { //acknowledgement packet
  TPacket okPacket;
  okPacket.packetType = PACKET_TYPE_RESPONSE;
  okPacket.command = RESP_OK;
  sendResponse(&okPacket);  
}

/**************************************************************************************************
 * Handling of TPacket
**************************************************************************************************/
void handleCommand(TPacket *command) {
  switch(command->command) {
    case COMMAND_FORWARD: // For movement commands, param[0] = distance, param[1] = speed.
        sendOK();
        forwardH((float) command->params[0], (float) command->params[1]);
    break;
    
    case COMMAND_REVERSE:
        sendOK();
        reverseH((float) command->params[0], (float) command->params[1]);
    break;
    
    case COMMAND_TURN_LEFT:
        sendOK();
        leftH((float) command->params[0], (float) command->params[1]);
    break;
    
    case COMMAND_TURN_RIGHT:
        sendOK();
        rightH((float) command->params[0], (float) command->params[1]);
    break;
         
    default:
      sendBadCommand();
  }
}

void handlePacket(TPacket *packet) {
  switch(packet->packetType) {
    case PACKET_TYPE_COMMAND:
      handleCommand(packet);
      break;

    case PACKET_TYPE_RESPONSE:
      break;

    case PACKET_TYPE_ERROR:
      break;

    case PACKET_TYPE_MESSAGE:
      break;

    case PACKET_TYPE_HELLO:
      break;
  }
}
