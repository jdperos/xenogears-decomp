#include "common.h"

#include "field/main.h"
#include "psyq/libgpu.h"
#include "psyq/libgte.h"
#include "psyq/libetc.h"

INCLUDE_ASM("asm/field/nonmatchings/main/init", FieldInitializeControllersAndMouse);

INCLUDE_ASM("asm/field/nonmatchings/main/init", func_80071F64);

void FieldInitializeRenderContexts(void) {
    D_80059198 = 1;
    DrawSync(0);
    Vsync(0);
    InitGeom();
    SetGeomOffset(0xA0, 0x70);
    SetDefDrawEnv(&g_FieldRenderContexts[0].drawEnvs[0], 0, 0, 0x140, 0xE0);
    SetDefDrawEnv(&g_FieldRenderContexts[1].drawEnvs[0], 0, 0x100, 0x140, 0xE0);
    SetDefDrawEnv(&g_FieldRenderContexts[0].drawEnvs[1], 0, 0, 0x140, 0xE0);
    SetDefDrawEnv(&g_FieldRenderContexts[1].drawEnvs[1], 0, 0x100, 0x140, 0xE0);
    SetDefDispEnv(&g_FieldRenderContexts[0].dispEnv, 0, 0x100, 0x140, 0xE0);
    SetDefDispEnv(&g_FieldRenderContexts[1].dispEnv, 0, 0, 0x140, 0xE0);
    func_80071F64(0, 0, 0x140, 0xE0);
    func_80086D8C();

    g_FieldRenderContexts[0].drawEnvs[0].r0 = 0;
    g_FieldRenderContexts[0].drawEnvs[0].g0 = 0;
    g_FieldRenderContexts[0].drawEnvs[0].b0 = 0;
    g_FieldRenderContexts[1].drawEnvs[0].r0 = 0;
    g_FieldRenderContexts[1].drawEnvs[0].g0 = 0;
    g_FieldRenderContexts[1].drawEnvs[0].b0 = 0;
    g_FieldRenderContexts[0].drawEnvs[0].dtd = 1;
    g_FieldRenderContexts[1].drawEnvs[0].dtd = 1;
    
    Vsync(0);
    PutDispEnv(&g_FieldRenderContexts[1].dispEnv);
    PutDrawEnv(&g_FieldRenderContexts[1].drawEnvs[0]);
    func_8002DFF0(0x140, 0xF0);
}
