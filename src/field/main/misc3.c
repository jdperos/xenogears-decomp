#include "common.h"
#include "field/main.h"
#include "field/fade_effect.h"

INCLUDE_ASM("asm/field/nonmatchings/main/misc3", func_8006FDEC);

void FieldLZSSDecompress(void* _unused, void* pCompressed, void* pDecompressed) {
    LZSSDecompress(pCompressed, pDecompressed);
}

INCLUDE_ASM("asm/field/nonmatchings/main/misc3", func_800700B0);

INCLUDE_ASM("asm/field/nonmatchings/main/misc3", func_80070340);

INCLUDE_ASM("asm/field/nonmatchings/main/misc3", func_80070488);

INCLUDE_ASM("asm/field/nonmatchings/main/misc3", func_80070508);

INCLUDE_ASM("asm/field/nonmatchings/main/misc3", func_80070560);

INCLUDE_ASM("asm/field/nonmatchings/main/misc3", func_80070594);

INCLUDE_ASM("asm/field/nonmatchings/main/misc3", func_800705DC);

INCLUDE_ASM("asm/field/nonmatchings/main/misc3", func_80070C84);

// Yolo
INCLUDE_ASM("asm/field/nonmatchings/main/misc3", func_80070CC8);

extern int D_800ADC04; // Fade Status?
extern short D_800ADC08;

void func_80071A64(void) {
    FieldFadeInitialize(0);
    FieldFadeInitialize(1);
}

void FieldFadeUpdate(int index) {
    if (g_FieldFadeEffect.fades[index].isVisible) {  

        if (g_FieldFadeEffect.fades[index].duration <= 0) {
            g_FieldFadeEffect.fades[index].duration = 0;
            if (
                D_800ADC08 != 1 &&
                g_FieldFadeEffect.fades[index].r0 == 0 &&
                g_FieldFadeEffect.fades[index].g0 == 0 &&
                g_FieldFadeEffect.fades[index].b0 == 0
            ) {
                g_FieldFadeEffect.fades[index].isVisible = 0;
            }
        } else {
            g_FieldFadeEffect.fades[index].r0 += g_FieldFadeEffect.fades[index].redDelta;
            if ((g_FieldFadeEffect.fades[index].r0 >> 8) >= 0x100) {
                g_FieldFadeEffect.fades[index].r0 = 0xFF00;
            }
            if (g_FieldFadeEffect.fades[index].r0 < 0x0) {
                g_FieldFadeEffect.fades[index].r0 = 0x0;
            }
            
            g_FieldFadeEffect.fades[index].g0 += g_FieldFadeEffect.fades[index].greenDelta;
            if ((g_FieldFadeEffect.fades[index].g0 >> 8) >= 0x100) {
                g_FieldFadeEffect.fades[index].g0 = 0xFF00;
            }
            if (g_FieldFadeEffect.fades[index].g0 < 0x0) {
                g_FieldFadeEffect.fades[index].g0 = 0x0;
            }

            g_FieldFadeEffect.fades[index].b0 += g_FieldFadeEffect.fades[index].blueDelta;
            if ((g_FieldFadeEffect.fades[index].b0 >> 8) >= 0x100) {
                g_FieldFadeEffect.fades[index].b0 = 0xFF00;
            }
            if (g_FieldFadeEffect.fades[index].b0 < 0x0) {
                g_FieldFadeEffect.fades[index].b0 = 0x0;
            }

            g_FieldFadeEffect.fades[index].duration--;
        }
    }
}

void FieldFadeUpdateAndDraw(u_long *ot) {
    if (D_800ADC04 == 2) {
        FieldFadeUpdate(0);
        FieldFadeUpdate(1);
    }
    FieldFadeDraw(ot, g_FieldCurRenderContextIndex);
}

void func_80071D08(int index, int duration, int red, int green, int blue, int semitransparency) {
    g_FieldFadeEffect.fades[index].duration = duration;
    g_FieldFadeEffect.fades[index].isVisible = 1;
    g_FieldFadeEffect.fades[index].semitransparency = semitransparency;
    g_FieldFadeEffect.fades[index].redDelta = (red * 0x100 - g_FieldFadeEffect.fades[index].r0) / duration;
    g_FieldFadeEffect.fades[index].greenDelta = (green * 0x100 - g_FieldFadeEffect.fades[index].g0) / duration;
    g_FieldFadeEffect.fades[index].blueDelta = (blue * 0x100 - g_FieldFadeEffect.fades[index].b0) / duration;
}

void func_80071DCC(int duration) {
    if (D_800ADC08 != 1) {
        D_800ADC08 = 1;
        if (D_800ADC04 == 2) {
            g_FieldFadeEffect.fades[0].b0 = 0;
            g_FieldFadeEffect.fades[0].g0 = 0;
            g_FieldFadeEffect.fades[0].r0 = 0;
            g_FieldFadeEffect.fades[0].duration = duration;
            g_FieldFadeEffect.fades[0].isVisible = 1;
            g_FieldFadeEffect.fades[0].semitransparency = 2;
            g_FieldFadeEffect.fades[0].blueDelta = 0xFF00 / duration;
            g_FieldFadeEffect.fades[0].greenDelta = 0xFF00 / duration;
            g_FieldFadeEffect.fades[0].redDelta = 0xFF00 / duration;
        }
    }
}

void func_80071E58(int duration) {
    if (D_800ADC08) {
        D_800ADC08 = 0;
        if (D_800ADC04 == 2) {
            g_FieldFadeEffect.fades[0].b0 = 0xFF00;
            g_FieldFadeEffect.fades[0].g0 = 0xFF00;
            g_FieldFadeEffect.fades[0].r0 = 0xFF00;
            g_FieldFadeEffect.fades[0].duration = duration;
            g_FieldFadeEffect.fades[0].isVisible = 1;
            g_FieldFadeEffect.fades[0].semitransparency = 2;
            g_FieldFadeEffect.fades[0].blueDelta = -0x10000 / duration;
            g_FieldFadeEffect.fades[0].greenDelta = -0x10000 / duration;
            g_FieldFadeEffect.fades[0].redDelta = -0x10000 / duration;
        }
    }
}
