#ifndef _XENO_FIELD_GRAPHICS_H
#define _XENO_FIELD_GRAPHICS_H

#include "psyq/libgpu.h"

typedef struct {
    DR_MODE drModes[0x21 * 2];
    SPRT sprites[0x21 * 2];
} SpriteList;

#endif