#ifndef _XENO_FIELD_FADE_EFFECT_H
#define _XENO_FIELD_FADE_EFFECT_H

#include "psyq/libgpu.h"

typedef struct {
    DR_MODE drawModes[2];
    TILE tiles[2];
    int r0;
    int g0;
    int b0;
    int redDelta;
    int greenDelta;
    int blueDelta;
    short semitransparency;
    short isVisible;
    short duration;
    short _pad;
} FieldFade;

typedef struct {
    char _pad[0x4C];
    FieldFade fades[2];
} FieldFadeEffect;

extern FieldFadeEffect g_FieldFadeEffect;

#endif