#ifndef _XENO_FIELD_TEXT_BOX_H
#define _XENO_FIELD_TEXT_BOX_H

#include "psyq/libgpu.h"

typedef struct {
    short visibility;
    short startPosition;
    short maxPosition;
    short curPosition;
    DR_MODE drawModes[2];
    SPRT sprites[2];
} FieldTextBoxCursor;

typedef struct {
    short visibility;
    u_char _pad[2];
    DR_MODE drawModes[2];
    SPRT sprites[2];
} FieldTextBoxContinueArrow;

typedef struct {
    DR_MODE drawModes1[0xA]; // CTX 1
    DR_MODE drawModes2[0xA]; // CTX 2
    SPRT sprites1[0xA]; // CTX 1
    SPRT sprites2[0xA]; // CTX 2
} FieldTextBoxBorders;

typedef struct {
    DR_MODE drawModes[2];
    TILE tiles[2];
} FieldTextBoxBackground;

typedef struct {
    DR_MODE drawModes[2];
    POLY_FT4 polys[2];
    u_char shouldRenderPortrait;
    u_char portraitID;
    u_char _pad[0x2];
} FieldTextBoxPortrait;

// FieldTextBox Size 0x498
typedef struct {
    DR_MODE drawModes[2];

    // 0x18
    char _pad[0xAC];

    // Background, 0xC4
    FieldTextBoxBackground background;

    // Borders, 0xFC
    FieldTextBoxBorders borders;

    // Cursor, 0x37C
    FieldTextBoxCursor cursor;

    // Continue Arrow, 0x3C4
    FieldTextBoxContinueArrow continueArrow;
    short windowOpenTimer;
    short continueArrowTimer;

    /*
    0x0001 - set when top message init and bottom.
    0x0020 - flip face.
    0x0040 - don't render border and background and continue arrow.
    0x0080 - set when bottom message init.
    */
    short flags; // 0x40C
    short visibility; // 0 = Visible, -1 = Hidden
    short order; // 0-top. 0xffff if window not inited
    short unk412;
    short status; // Set to 0 to close window. Usually 0xffff (not used in usual window render)
    short ownerActorID;
    short talkingActorID;
    int positionOffsetX;
    int positionOffsetY;
    int positionOffsetDeltaX;
    int positionOffsetDeltaY;

    // Portrait, 0x42C
    FieldTextBoxPortrait portrait;
} FieldTextBox;

extern FieldTextBox g_FieldTextBoxes[4];

#endif