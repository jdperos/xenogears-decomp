#ifndef _XENO_FONT_H
#define _XENO_FONT_H

#include "psyq/libgpu.h"

// This struct is just a header for the font,
// letter data follows directly after at the end
typedef struct {
    // Flags
    // 0x1: Indicates if letter size should be 0x10 or 0x20
    //
    // 0x4: Determines heap allocation mode
    short flags;

    short texpage;
    void* pPacketBuffer1;
    void* pPacketBuffer2;
    short startX;
    short startY;
    short areaWidth;
    short areaHeight;
    short letterWidth;
    short rowHeight;
    char defaultColor[3]; // r, g, b
    char primitiveCode;
    DR_TPAGE texpageSettings1;
    DR_TPAGE texpageSettings2;
    short maxNumLetters;

    // Flags 2
    // 0x1 - current render buffer.
    // 0x2 - 16x16 letters.
    // 0x4 - use uppercase letter (only if >= 0x60).
    // 0x8 - use letter width.
    short flags2;
    short curX;
    short curY;
    short curNumLetters;
    short nextRowX;
    void* pLetterRenderPacket;
    short letterClutIds[4];
    TILE bgTile1;
    TILE bgTile2;
    char arr[0x60];
    void* pPacket1Start;
    void* pPacket2Start;
    short curPosX;
    short curPosY;
    short newRowX;
    char vramLetterOffsetX;
    char _pad;
} Font;

// Incomplete
typedef struct {
    short mode;
    char letterWidth;
    char rowHeight;
} FontFile;

extern Font* g_Font;

#endif