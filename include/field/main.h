#ifndef _XENO_FIELD_MAIN_H
#define _XENO_FIELD_MAIN_H

#include "psyq/libgpu.h"
#include "psyq/libgte.h"

#define setRGB(obj, _color) obj.r0 = obj.g0 = obj.b0 = _color

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
    SVECTOR vecs[4];
    POLY_FT4 polys[2];
} Quad;

typedef struct {
    short x;
    short y;
    short z;
    short w;
} SVec4;

typedef struct {
    // 0x800b11ac
    DR_MODE drawModes[10];
    
    // 0x800b1224
    RECT rects[10];

    // 0x800b1274
    POLY_FT4 polygons[10];

    // 0x800b1404
    SVECTOR vectors[20]; // Vectors for RotAverage4
} ZoomFadeEffect;

extern ZoomFadeEffect g_FieldZoomFadeEffect;

extern int D_80059198; // Has initialized render contexts?

#define SYSTEM_MODE_PC_HDD 0x0
#define SYSTEM_MODE_CD_ROM 0x1

extern int g_FieldSystemMode;

extern int g_FieldCurRenderContextIndex;
extern int g_FieldRenderContextUseOT2;
extern RenderContext* g_FieldCurRenderContext;
extern RenderContext g_FieldRenderContexts[2];

#endif