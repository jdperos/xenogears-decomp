#include "common.h"

#include "system/font.h"
#include "system/memory.h"

extern void* g_FontClutData;
extern RECT g_FontClutDest;
extern void* D_800593A0; // Non-heap allocated Font?

void FontLoadClut(short arg0, short arg1) {
    short* pClutData;
    int i;
    int j;

    pClutData = &g_FontClutData;
    
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

    LoadImage(&g_FontClutDest, &g_FontClutData);
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

void FontStoreCurrentPositions(void) {
    g_Font->storedPosX = g_Font->curX;
    g_Font->storedPosY = g_Font->curY;
    g_Font->storedRowX = g_Font->nextRowX;
}

void FontRestoreCurrentPositions(void) {
    g_Font->curX = g_Font->storedPosX;
    g_Font->curY = g_Font->storedPosY;
    g_Font->nextRowX = g_Font->storedRowX;
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/font", func_8003700C);

void FontSetPositionsByOffset(short xOffset, short yOffset) {
    if (g_Font) {
        g_Font->curX = g_Font->startX + xOffset;
        g_Font->curY = g_Font->startY + yOffset;
    }
}

void func_8003708C(int offsetX, int offsetY) {
    short nNewPosX;

    if (g_Font) {
        if (offsetX < 0)
            offsetX = 0;
        
        if (offsetY < 0)
            offsetY = 0;
        
        nNewPosX = g_Font->startX + offsetX;
        g_Font->curX = nNewPosX;
        g_Font->nextRowX = nNewPosX;
        g_Font->curY = g_Font->startY + offsetY;
    }
}

void FontAddLetterPrimitive(int letter) {
    Font* pFont;
    int nAsciiLetter;
    short nU0;
    short nV0;
    int nLetterWidth;

    pFont = g_Font;
    nAsciiLetter = letter;
    
    if (pFont && 
        (pFont->curY + pFont->rowHeight) <= (pFont->startY + pFont->areaHeight) && 
        pFont->curNumLetters <= pFont->maxNumLetters
    ) {
        
        if (nAsciiLetter < 0x20) {
            if (nAsciiLetter == '\n') {
                pFont->curX = pFont->nextRowX;
                pFont->curY = pFont->curY + pFont->rowHeight;
            }     
        } else {
            // Uppercase
            if ((pFont->flags2 & 4) && (nAsciiLetter >= 0x60))
                nAsciiLetter -= 0x20;
            nAsciiLetter = nAsciiLetter - 0x20;
            
            if (pFont->flags2 & 8)
                nLetterWidth = pFont->arr[nAsciiLetter];
            else
                nLetterWidth = pFont->letterWidth;
            
            if ((pFont->curX + nLetterWidth) >= (pFont->startX + pFont->areaWidth)) {
                if (!(pFont->flags & 8)) {
                    pFont->curX = pFont->nextRowX;
                    pFont->curY = pFont->curY + pFont->rowHeight;
                } else {
                    return;
                }
            }
            
            if (nAsciiLetter) {
                // Color + primitive code
                *(int*)&pFont->pLetterRenderPacket->r0 = *(int*)&pFont->defaultColor;

                setXY0Fast(pFont->pLetterRenderPacket, pFont->curX, pFont->curY);

                // Letter size flag
                if (pFont->flags2 & 2) {
                    pFont->pLetterRenderPacket->clut = pFont->letterClutIds[(nAsciiLetter & 0x18) >> 3];

                    nU0 = (nAsciiLetter & 7) * 0x10;
                    nV0 = (nAsciiLetter & 0x60) >> 1;
                    setUV0Fast(pFont->pLetterRenderPacket, nU0, pFont->vramLetterOffsetX + nV0);
                } else {
                    pFont->pLetterRenderPacket->clut =  pFont->letterClutIds[(nAsciiLetter & 0x30) >> 4];

                    nU0 = (nAsciiLetter & 0xF) * 8;
                    nV0 = (nAsciiLetter & 0xC0) >> 3;
                    setUV0Fast(pFont->pLetterRenderPacket, nU0, pFont->vramLetterOffsetX + nV0);
                }
                
                pFont->pLetterRenderPacket++;
                pFont->curNumLetters++;
            }
            
            pFont->curX += nLetterWidth;
        }
    }
}

void FontResetLetterRendering(void) {
    short nStartX;
    short nStartY;
    int nContext;
    u_char nPrimitiveCode;
    
    nContext = g_Font->flags2 & 1;
    nStartY = g_Font->startY;
    nPrimitiveCode = g_Font->primitiveCode & 0xFE;
    g_Font->pLetterRenderPacket = g_Font->pPrimBuffers[nContext];
    nStartX = g_Font->startX;
    g_Font->curNumLetters = 0;
    g_Font->curY = nStartY;
    g_Font->storedPosY = nStartY;
    g_Font->curX = nStartX;
    g_Font->storedPosX = nStartX;
    g_Font->nextRowX = nStartX;
    g_Font->storedRowX = nStartX;
    g_Font->primitiveCode = nPrimitiveCode;
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/font", func_80037324);

void func_8003747C(void* pFont) {
    D_800593A0 = pFont;
}

void FontFree(void) {
    if (g_Font) {
        g_HeapDebugPrintfFn = func_8003700C; // ScreenPrintf
        if (D_800593A0 == NULL)
            HeapFree(g_Font);
        g_Font = NULL;
    }
    D_800593A0 = NULL;
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/font", FontLoadFont);
