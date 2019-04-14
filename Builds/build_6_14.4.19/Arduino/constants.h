/**************************************************************************************************
 * constants.h
 * Constant Header File
 * This lib containts all the packet types, commands and status constants
 * 
 * Contents:
 * Motor Pin Declarations (motor.h / motorA.h)
 * Hall Effect Const Declarations (motor.h)
 * Dir Declarations (unused)
 * TPacket Declaration (packet.h)
 * TPacket Enum Declarations (packet.h)
**************************************************************************************************/

/*|motor.h|****************************************************************************************
**************************************************************************************************/

//motor pins
#define M1F 6
#define M1R 5
#define M2F 9
#define M2R 10

#define LMOTORCOMP 1.0
#define RMOTORCOMP 0.98
#define COURSECOMP 0.98

//hall effect constants
#define COUNTS_PER_REV 188 //Num of Ticks per Revolution
#define WHEEL_CIRC 20 //Wheel circumference in cm.
#define COUNTS_PER_RATURN 58 //Number of ticks per right angle turn 

//motor.h Global Declarations
static volatile unsigned long leftCount = 0; //Store encoder tick count
static volatile unsigned long rightCount = 0;
static volatile double leftAngle = 0; //Revolution count per wheel
static volatile double rightAngle = 0;
static volatile unsigned long forwardDist = 0;
static volatile unsigned long reverseDist = 0;
static volatile unsigned long maxCount = 0; //max count to stop
static volatile unsigned long motorSpeed = 0;
static volatile unsigned long netAngle = 0; //bet angle movement
static volatile long distFront = 0;

void resetGlobalsH();

//Direction values
/*typedef enum dir {
  STOP,
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT
} dir;*/

typedef enum {
  STOP=0,
  FORWARD=1,
  BACKWARD=2,
  LEFT=3,
  RIGHT=4
} TDirection;
volatile TDirection dir = STOP;

/*|packet.h|***************************************************************************************
**************************************************************************************************/
char *message = "[AR] NULL"; //any miscellaneous messages to print

void sendTooClose();
void sendMsgAuto();

#ifndef __CONTROL_H__
#define __CONTROL_H__


#define MAX_STR_LEN   32 //Packet is (1 + 1 + 2 + 32 + 16 * 4) = 100 bytes

typedef struct {
  char packetType;
  char command;
  char dummy[2]; // Padding to make up 4 bytes
  char data[MAX_STR_LEN]; // String data
  uint32_t params[16];
} TPacket;

#endif

//packet enumerations

#ifndef __CONSTANTS_INC__
#define __CONSTANTS_INC__

#define PACKET_SIZE 140
#define MAX_DATA_SIZE 128

typedef enum { //Packet types
  PACKET_TYPE_COMMAND = 0,
  PACKET_TYPE_RESPONSE = 1,
  PACKET_TYPE_ERROR = 2,
  PACKET_TYPE_MESSAGE = 3,
  PACKET_TYPE_HELLO = 4
} TPacketType;

typedef enum { //Response types. This goes into the command field
  RESP_OK = 0,
  RESP_STATUS=1,
  RESP_BAD_PACKET = 2,
  RESP_BAD_CHECKSUM = 3,
  RESP_BAD_COMMAND = 4,
  RESP_BAD_RESPONSE = 5,
  RESP_TOO_CLOSE = 6
} TResponseType;

typedef enum { //Commands
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

/*|TResult is defined in <serialize.h>, declaration here for reference|****************************
**************************************************************************************************/
/*typedef enum {
  PACKET_OK = 0,
  PACKET_BAD = 1,
  PACKET_CHECKSUM_BAD = 2,
  PACKET_INCOMPLETE = 3,
  PACKET_COMPLETE = 4
} TResult;*/


#endif

/*|serialize.h|************************************************************************************
**************************************************************************************************/
#ifndef __SERIALIZE__
#define SERIALIZE

#include <stdlib.h>

int serialize(char *buffer, void *dataStructure, size_t size);
TResult deserialize(const char *buffer, int len, void *output);

#endif

//ultrasonic.h

#define USTRIG 12 //Ultrasonic Sensor Trigger Pin
#define USECHO 13 //Echo Pin


/*|lightsensor.h|**********************************************************************************
**************************************************************************************************/
#include <Wire.h>
#include "Adafruit_TCS34725.h"

void calibrateLS();
void readColor();
