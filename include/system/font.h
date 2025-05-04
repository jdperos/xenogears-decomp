#ifndef _XENO_FONT_H
#define _XENO_FONT_H

#include "psyq/libgpu.h"

// Should probably be moved to something like system/graphics.h at some point
#define setXY0Fast(p,_x0,_y0) *(int*)&(p)->x0 = (_x0) | ((_y0) << 0x10)
#define setWHFast(p,_w,_h) *(int*)&(p)->w = (_w) | ((_h) << 0x10)
#define setUV0Fast(p,_u0,_v0) *(short*)&(p)->u0 = (_u0) | ((_v0) << 0x8)

// This struct is just a header for the font,
// letter data follows directly after at the end
typedef struct {
    // Flags
    // 0x1: Indicates if letter size should be 0x10 or 0x20
    //
    // 0x4: Determines heap allocation mode
    short flags;

    short texpage;
    void* pPrimBuffers[2];
    short startX;
    short startY;
    short areaWidth;
    short areaHeight;
    short letterWidth;
    short rowHeight;
    char r;
    char g;
    char b;
    char primitiveCode;
    DR_TPAGE texpageSettings[2];
    short maxNumLetters;

    // Flags 2
    // 0x1 - current render buffer.
    // 0x2 - 16x16 letters.
    // 0x4 - use uppercase letter (only if >= 0x60).
    // 0x8 - use letter width.
    short letterFlags;
    short curX;
    short curY;
    short curNumLetters;
    short nextRowX;
    SPRT_16* pLetterRenderPacket;
    short letterClutIds[4];
    TILE bgTiles[2];
    char arr[0x60];
    void* pPrimBuffersStart[2];
    short storedPosX;
    short storedPosY;
    short storedRowX;
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