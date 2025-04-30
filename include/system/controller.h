#ifndef _XENO_CONTROLLER_INPUT_H
#define _XENO_CONTROLLER_INPUT_H

// Internal controller type masks
#define CONTROLLER_INTERNAL_TYPE_MOUSE 0x10
#define CONTROLLER_INTERNAL_TYPE_DIGITAL_PAD 0x40
#define CONTROLLER_INTERNAL_TYPE_ANALOG_PAD 0x50
#define CONTROLLER_INTERNAL_TYPE_ANALOG_STICK 0x70

#define CONTROLLER_STATUS_SUCCESS 0x00
#define CONTROLLER_STATUS_ERROR 0xFF

// External Controller types
#define CONTROLLER_TYPE_ERROR -1
#define CONTROLLER_TYPE_NONE 0x0
#define CONTROLLER_TYPE_DIGITAL_PAD 0x1
#define CONTROLLER_TYPE_MOUSE 0x2
#define CONTROLLER_TYPE_ANALOG_PAD 0x3
#define CONTROLLER_TYPE_ANALOG_STICK 0x4

#define CONTROLLER_BUFFER_SIZE 0x22

/*
Controller buffer follows this structure:

typedef struct {
  u8 status:8;

  u8 recvSize:4;
  u8 type:4;
  
  u8 select:1;
  u8 na2:1;
  u8 na1:1;
  u8 start:1;
  u8 up:1;
  u8 right:1;
  u8 down:1;
  u8 left:1;

  u8 l2:1;
  u8 r2:1;
  u8 l1:1;
  u8 r1:1;
  u8 triangle:1;
  u8 circle:1;
  u8 ex:1;
  u8 square:1;
} ControllerBuffer;
*/

// Controller buffer start
extern u8 D_800625FC[];
extern u8 D_800625FD[];

#endif