#ifndef _XENO_FIELD_MAIN_H
#define _XENO_FIELD_MAIN_H

#include "psyq/libgpu.h"

typedef struct {
    DRAWENV drawEnvs[2];
    DISPENV dispEnv;
    u_long ot1[0x1000];
    int unk1;
    u_long ot2[0x1000];
    int unk2;
    u_long ot3[0x8];
} RenderContext;

typedef struct {
    // 0x800b11ac
    DR_MODE drawModes[5][2];
    
    // 0x800b1224
    RECT rects[5][2];

    // 0x800b1274
    POLY_FT4 prims[5][2];

    // 0x800b1404
    SVECTOR vecs[5][4]; // Vectors for RotAverage4
} ZoomFadeEffect;

extern int D_80059198; // Has initialized render contexts?

#define SYSTEM_MODE_PC_HDD 0x0
#define SYSTEM_MODE_CD_ROM 0x1

extern int g_FieldSystemMode;

extern int g_FieldCurRenderContextIndex;
extern int g_FieldRenderContextUseOT2;
extern RenderContext* g_FieldCurRenderContext;
extern RenderContext g_FieldRenderContexts[2];

#endif