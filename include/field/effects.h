#ifndef _XENO_FIELD_EFFECTS_H
#define _XENO_FIELD_EFFECTS_H

#include "psyq/libgpu.h"

typedef struct {
    short isActive;
    short isFinished;
    short duration;
    char _pad[2];
    int v1;
    int v2;
    int v3;
    int v4;
    int v5;
    int v6;
    int delta1;
    int delta2;
    int delta3;
    int delta4;
    int delta5;
    int delta6;
    short unk38;
    short unk3C;
    DR_MOVE* buffer0;
    DR_MOVE* buffer1;
    POLY_FT4* buffer2;
    POLY_FT4* buffer3;
} FieldDistortion;

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
    FieldDistortion distortion;
    FieldFade fades[2];
} FieldEffects;

extern FieldEffects g_FieldEffects;

#endif