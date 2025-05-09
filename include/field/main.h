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

extern int D_80059198; // Has initialized render contexts?
extern RenderContext g_FieldRenderContexts[2];

#endif