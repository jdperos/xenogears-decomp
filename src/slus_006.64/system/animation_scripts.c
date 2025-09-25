#include "common.h"
#include "field/actor.h"

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", func_8001FBA4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", func_8001FBE4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", func_80021AD8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", func_80021B04);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", func_80021B14);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", func_80021B24);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", func_80021B48);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", func_80021B6C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", func_80021B98);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", func_80021BCC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", func_80021BF0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", func_80021BF8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", func_80021C00);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", AnimScriptStackPopU8);
/*
Matches on GCC 2.7.2-970404, ASPSX 2.67
Co-Authored-By: Mc-muffin <Mc-muffin@users.noreply.github.com>

u_char AnimScriptStackPopU8(SpriteData* pSpriteData) {
    u_char nStackValue;

    nStackValue = pSpriteData->stack[pSpriteData->stackIndex];
    pSpriteData->stackIndex++;
    return nStackValue;
}
*/

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", AnimScriptStackPopU16);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", AnimScriptStackPopU24);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", AnimScriptStackPushU8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", AnimScriptStackPushU16);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", AnimScriptStackPushU24);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", func_80021D3C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", func_80021D50);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", func_80021EBC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", func_80021FB8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", func_80021FC0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", func_80021FE0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", func_80022000);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", func_80022038);


INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", func_80022090);

// Set animation package
INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", func_80022224);

extern s32 func_8001EE68(void*);
extern void func_80022224(SpriteAnimPackage*, void*, SVEC2, SVEC2, u32);
extern u8 D_800591AD;

// >= ASPSX 2.56, likely GCC 2.7.2-970404, ASPSX 2.67
void func_800222BC(SpriteData* pSprite, SpriteAnimPackageFileHeader* pAnimPackageFile) {
    SpriteAnimPackage* pAnimPackage;
    SVEC2 unused[2];

    pAnimPackage = pSprite->pVramData;
    if (pAnimPackageFile) {
        if (pAnimPackageFile != pSprite->pCurAnimFile) {
            func_80022224(pAnimPackage, pAnimPackageFile, pAnimPackage->tex, pAnimPackage->clut, pSprite->field_0x3C_1);
            pSprite->pCurAnimFile = pAnimPackageFile;
            pSprite->field_0x3C_3 = 1;
        }

        // Is battle?
        if (D_800591AD) {
            // Is not VRAM Pre-backed?
            if (!func_8001EE68(pAnimPackage->pFrames)) {
                pAnimPackage->tex.y = 0x100;
                pAnimPackage->tex.x = 0x300;
                return;
            }
            pAnimPackage->tex = *(SVEC2*)(pSprite->field_0x7C + 0xE);
        }
    }
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", func_800223B0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", func_80022660);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", func_80022974);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/animation_scripts", func_80022A00);
