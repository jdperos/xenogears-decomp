#ifndef _XENO_FIELD_GRAPHICS_H
#define _XENO_FIELD_GRAPHICS_H

#include "psyq/libgpu.h"

typedef struct {
    DR_MODE drModes[0x21][2];
    SPRT sprites[0x21][2];
} SpriteList;

typedef struct {
    DR_MODE drModes[0x10][2];
    SPRT sprites[0x10][2];
} SpriteList2;

typedef struct {
    DR_MODE drModes[5][2];
    SPRT sprites[5][2];
} SpriteList3;


typedef struct {
    DR_MODE drModes[5][2];
    RECT rects[5][2];
    POLY_FT4 polys[5][2];
} PolyList2;

#endif