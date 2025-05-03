#include "common.h"

#include "system/font.h"

extern void* D_80050598; // Font CLUT Data?
extern RECT D_80059398; // Font CLUT Dest?

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

int FontGetDefaultLetterWidth(void) {
    return g_Font->letterWidth;
}

int FontGetDefaultLetterHeight(void) {
    return g_Font->rowHeight;
}

int FontGetCurLetterX(void) {
    return g_Font->curX;
}

int FontGetCurLetterY(void) {
    return g_Font->curY;
}

int FontGetCurLetterCount(void) {
    return g_Font->curNumLetters;
}

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
