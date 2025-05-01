#ifndef _XENO_CONTROLLER_INPUT_H
#define _XENO_CONTROLLER_INPUT_H

// Internal controller type masks
#define CONTROLLER_INTERNAL_TYPE_MOUSE 0x10
#define CONTROLLER_INTERNAL_TYPE_DIGITAL_PAD 0x40
#define CONTROLLER_INTERNAL_TYPE_ANALOG_PAD 0x50
#define CONTROLLER_INTERNAL_TYPE_ANALOG_STICK 0x70

// Controller Status values
#define CONTROLLER_STATUS_SUCCESS 0x0
#define CONTROLLER_STATUS_ERROR 0xFF

// External Controller types
#define CONTROLLER_TYPE_ERROR -1
#define CONTROLLER_TYPE_NONE 0x0
#define CONTROLLER_TYPE_DIGITAL_PAD 0x1
#define CONTROLLER_TYPE_MOUSE 0x2
#define CONTROLLER_TYPE_ANALOG_PAD 0x3
#define CONTROLLER_TYPE_ANALOG_STICK 0x4

#define CONTROLLER_BUFFER_SIZE 0x22

// Controller buffer indices
#define CONTROLLER_STATUS 0x0
#define CONTROLLER_TYPE 0x1
#define CONTROLLER_BUTTONS_1 0x2
#define CONTROLLER_BUTTONS_2 0x3

// Special button bits for analog stick controllers
#define CTRL_ANALOG_STICK_BTN_L1 0x2
#define CTRL_ANALOG_STICK_BTN_SQUARE 0x4
#define CTRL_ANALOG_STICK_BTN_TRIANGLE 0x8
#define CTRL_ANALOG_STICK_BTN_R1 0x10
#define CTRL_ANALOG_STICK_BTN_R2 0x80

// Button State Values
#define CTRL_BTN_L2 0x1
#define CTRL_BTN_R2 0x2
#define CTRL_BTN_L1 0x4
#define CTRL_BTN_R1 0x8
#define CTRL_BTN_TRIANGLE 0x10
#define CTRL_BTN_CIRCLE 0x20
#define CTRL_BTN_CROSS 0x40
#define CTRL_BTN_SQUARE 0x80
#define CTRL_BTN_SELECT 0x100
#define CTRL_BTN_L3 0x200
#define CTRL_BTN_R3 0x400
#define CTRL_BTN_START 0x800
#define CTRL_BTN_UP 0x1000
#define CTRL_BTN_RIGHT 0x2000
#define CTRL_BTN_DOWN 0x4000
#define CTRL_BTN_LEFT 0x8000

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

extern u_char g_ControllerType;

// Controller buffer start
extern u_char D_800625FC[];
extern u_char D_800625FD[];


extern u_char g_ControllerStickToAnalogX[0x10];
/*
u_char g_ControllerStickToAnalogX[0x10] = {
    0x80, 0x80, 0xFF, 0xFF,
    0x80, 0x80, 0xFF, 0x80,
    0x0, 0x0, 0x80, 0x80,
    0x0, 0x80, 0x80, 0x80
}
*/

extern u_char g_ControllerStickToAnalogY[0x10];
/*
u_char g_ControllerStickToAnalogY[0x10] = {
    0x80, 0x0, 0x80, 0x0,
    0xFF, 0x80, 0xFF, 0x80,
    0x80, 0x0, 0x80, 0x80,
    0xFF, 0x80, 0x80, 0x80
}
*/

extern u_char g_ControllerButtonMappings[8];
/*
u_char g_ControllerButtonMappings[8] = {
  0x0, 0x1, 0x2, 0x3,
  0x4, 0x5, 0x6, 0x7
};
*/

extern u_short g_ControllerButtonMasks[8];
/*
u_short g_ControllerButtonMasks[8] = {
  CTRL_BTN_CIRCLE, CTRL_BTN_CROSS, CTRL_BTN_TRIANGLE, CTRL_BTN_SQUARE,
  CTRL_BTN_L1, CTRL_BTN_L2, CTRL_BTN_R1, CTRL_BTN_R2
};
*/

#endif