#include "common.h"

#include "psyq/libgpu.h"

extern void* D_80050598;
extern RECT D_80059398;

void FontLoadClut(short arg0, short arg1) {
    short* pClutData;
    int i;
    int j;

    pClutData = &D_80050598;
    
    for (i = 0; i < 8; i++) {
        *pClutData++ = arg1;
        *pClutData++ = arg0;
    }
    
    if (i < 0xC) {
        while (i < 0xC) {
            *pClutData++ = arg1;
            *pClutData++ = arg1;
            *pClutData++ = arg0;
            *pClutData++ = arg0;
            i++;
        };
    }
    
    while (i < 0xE) {
        for (j = 3; j >= 0; j--)
            *pClutData++ = arg1;

        for (j = 3; j >= 0; j--)
            *pClutData++ = arg0;
        
        i++;
    }

    for (j = 7; j >= 0; j--)
        *pClutData++ = arg1;

    for (j = 8; j < 0x10; j++)
        *pClutData++ = arg0;

    LoadImage(&D_80059398, &D_80050598);
}


INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/font", func_80036F44);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/font", func_80036F5C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/font", func_80036F74);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/font", func_80036F8C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/font", func_80036FA4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/font", func_80036FBC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/font", func_80036FE4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/font", func_8003700C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/font", func_80037058);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/font", func_8003708C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/font", func_800370DC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/font", func_800372CC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/font", func_80037324);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/font", func_8003747C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/font", func_8003748C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/font", FontLoadFont);
