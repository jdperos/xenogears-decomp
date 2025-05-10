#include "common.h"
#include "field/main.h"
#include "psyq/libgpu.h"

INCLUDE_ASM("asm/field/nonmatchings/main/misc2", func_80072140);

INCLUDE_ASM("asm/field/nonmatchings/main/misc2", func_80072150);

INCLUDE_ASM("asm/field/nonmatchings/main/misc2", func_80072254);

INCLUDE_ASM("asm/field/nonmatchings/main/misc2", func_800722F4);

INCLUDE_ASM("asm/field/nonmatchings/main/misc2", func_8007234C);

INCLUDE_ASM("asm/field/nonmatchings/main/misc2", func_80072398);

INCLUDE_ASM("asm/field/nonmatchings/main/misc2", func_800723E4);

INCLUDE_ASM("asm/field/nonmatchings/main/misc2", func_8007254C);

INCLUDE_ASM("asm/field/nonmatchings/main/misc2", func_800726E8);

INCLUDE_ASM("asm/field/nonmatchings/main/misc2", func_80072A38);

INCLUDE_ASM("asm/field/nonmatchings/main/misc2", func_80072D74);

INCLUDE_ASM("asm/field/nonmatchings/main/misc2", func_80073230);

INCLUDE_ASM("asm/field/nonmatchings/main/misc2", func_80073684);

INCLUDE_ASM("asm/field/nonmatchings/main/misc2", func_80073734);

INCLUDE_ASM("asm/field/nonmatchings/main/misc2", func_80073750);

INCLUDE_ASM("asm/field/nonmatchings/main/misc2", func_80073930);

INCLUDE_ASM("asm/field/nonmatchings/main/misc2", func_80073988);

INCLUDE_ASM("asm/field/nonmatchings/main/misc2", func_800739C0);

INCLUDE_ASM("asm/field/nonmatchings/main/misc2", func_80073E38);

void FieldClearAndSwapOTagInternal(void) {
    if (g_FieldSystemMode == SYSTEM_MODE_PC_HDD)
        asm("break 0x400");

    g_FieldCurRenderContextIndex = (g_FieldCurRenderContextIndex + 1) % 2;
    g_FieldCurRenderContext = &g_FieldRenderContexts[g_FieldCurRenderContextIndex];
    ClearOTagR(g_FieldCurRenderContext->ot3, 0x8);
}

void FieldClearAndSwapOTag(void) {
    FieldClearAndSwapOTagInternal();
    ClearOTagR(g_FieldCurRenderContext->ot1, 0x1000);
    if (g_FieldRenderContextUseOT2) {
        ClearOTagR(g_FieldCurRenderContext->ot2, 0x1000);
    }
}


INCLUDE_ASM("asm/field/nonmatchings/main/misc2", func_80074038);

INCLUDE_ASM("asm/field/nonmatchings/main/misc2", func_80074078);

INCLUDE_ASM("asm/field/nonmatchings/main/misc2", func_8007409C);

INCLUDE_ASM("asm/field/nonmatchings/main/misc2", func_80074108);

INCLUDE_ASM("asm/field/nonmatchings/main/misc2", func_8007469C);

INCLUDE_ASM("asm/field/nonmatchings/main/misc2", FieldPollControllers);

INCLUDE_ASM("asm/field/nonmatchings/main/misc2", func_800748E8);

INCLUDE_ASM("asm/field/nonmatchings/main/misc2", func_8007520C);

INCLUDE_ASM("asm/field/nonmatchings/main/misc2", func_800752C8);

INCLUDE_ASM("asm/field/nonmatchings/main/misc2", func_80075458);

INCLUDE_ASM("asm/field/nonmatchings/main/misc2", func_80075484);

INCLUDE_ASM("asm/field/nonmatchings/main/misc2", func_8007554C);

INCLUDE_ASM("asm/field/nonmatchings/main/misc2", func_80075910);

INCLUDE_ASM("asm/field/nonmatchings/main/misc2", func_800759E4);

INCLUDE_ASM("asm/field/nonmatchings/main/misc2", func_80075B08);

INCLUDE_ASM("asm/field/nonmatchings/main/misc2", func_80075B44);

INCLUDE_ASM("asm/field/nonmatchings/main/misc2", func_800764B4);

INCLUDE_ASM("asm/field/nonmatchings/main/misc2", func_80076A74);

INCLUDE_ASM("asm/field/nonmatchings/main/misc2", func_80076AC0);

INCLUDE_ASM("asm/field/nonmatchings/main/misc2", func_800771B0);

INCLUDE_ASM("asm/field/nonmatchings/main/misc2", func_800771F8);

INCLUDE_ASM("asm/field/nonmatchings/main/misc2", func_80077268);

INCLUDE_ASM("asm/field/nonmatchings/main/misc2", func_80077544);
