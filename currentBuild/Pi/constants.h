#ifndef __CONSTANTS_INC__
#define __CONSTANTS_INC__

/* 
 *  This file containts all the packet types, commands
 *  and status constants
 *  
 */

// Packet types
typedef enum {
  PACKET_TYPE_COMMAND = 0,
  PACKET_TYPE_RESPONSE = 1,
  PACKET_TYPE_ERROR = 2,
  PACKET_TYPE_MESSAGE = 3,
  PACKET_TYPE_HELLO = 4
} TPacketType;

// Response types. This goes into the command field
typedef enum {
  RESP_OK = 0,
  RESP_STATUS=1,
  RESP_BAD_PACKET = 2,
  RESP_BAD_CHECKSUM = 3,
  RESP_BAD_COMMAND = 4,
  RESP_BAD_RESPONSE = 5,
  RESP_TOO_CLOSE = 6
} TResponseType;


// Commands
// For direction commands, param[0] = distance in cm to move
// param[1] = speed
typedef enum {
  COMMAND_FORWARD = 0,
  COMMAND_REVERSE = 1,
  COMMAND_TURN_LEFT = 2,
  COMMAND_TURN_RIGHT = 3,
  COMMAND_SWING_LEFT = 4,
  COMMAND_SWING_RIGHT = 5,
  COMMAND_STOP = 6,
  COMMAND_FWD_NO_STOP = 7,
  COMMAND_GET_STATS = 8,
  COMMAND_CLEAR_STATS = 9,
  COMMAND_APPROACH = 10,
  COMMAND_GETDIST = 11,
  COMMAND_CALIBRATELS = 12,
  COMMAND_GETRGB = 13
} TCommandType;

#endif

