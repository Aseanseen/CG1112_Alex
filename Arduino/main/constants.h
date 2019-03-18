/**
 * constants.h
 * Constant Header File
 */

//motor pins
#define M1F 6
#define M1R 5
#define M2F 11
#define M2R 10

// Direction values
typedef enum dir {
  STOP,
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT
} dir;

//serial.h
#define PACKET_SIZE 12
