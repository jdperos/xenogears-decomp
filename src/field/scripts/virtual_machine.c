#include "common.h"
#include "field/main.h"
#include "field/actor.h"
#include "system/memory.h"
#include "psyq/libgpu.h"

extern Actor* g_FieldScriptVMCurActor;
extern void* g_FieldScriptVMCurScriptData;

void FieldScriptVMHandlerJmp(void) {
    g_FieldScriptVMCurActor->scriptInstructionPointer = FieldScriptVMGetInstructionArgument(1);
}

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A1E9C);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", FieldScriptVMRun);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A2030);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A22AC);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A2488);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A24C4);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A2714);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A28D4);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A2FC0);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A2FE0);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A3018);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", FieldScriptMemoryWriteU16);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A3090);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A30B4);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A30FC);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A31E8);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A3474);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A3C8C);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A3F4C);

//INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A4748);
void func_800A4748(void) {
    func_800A476C(0x2c0,0x100);
}

//INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A476C);
void func_800A476C(int x, int y) {
    RECT rect;

    rect.w = 0x140;
    rect.y = 0;
    rect.x = 0;
    rect.h = 0xe0;
    SetGeomScreen(0x200);
    MoveImage(&rect, x, y);
    FieldRenderSync();
}

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A47D4);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A484C);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A4CC4);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A4DAC);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A55B8);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A55C8);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A5600);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A56A8);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A5710);

void func_800A5774(int x, int y, int h) {
    RECT rect;
    int nSize;
    int i;
    u_long* pImageBuffer;
    u_long* pWorkBuffer;

    rect.x = x;
    rect.y = y;
    rect.w = 0x40;
    rect.h = h;
    pImageBuffer = HeapAlloc(h * 0x80, 0x1);
    StoreImage( &rect, pImageBuffer);
    DrawSync(0);
    
    pWorkBuffer = pImageBuffer;
    nSize = h * (0x80 / sizeof(u_long));
    for (i = 0; i < nSize; i += 8) {
        pWorkBuffer[0] |= 0x80008000;
        pWorkBuffer[1] |= 0x80008000;
        pWorkBuffer[2] |= 0x80008000;
        pWorkBuffer[3] |= 0x80008000;
        pWorkBuffer[4] |= 0x80008000;
        pWorkBuffer[5] |= 0x80008000;
        pWorkBuffer[6] |= 0x80008000;
        pWorkBuffer[7] |= 0x80008000; 
        pWorkBuffer += 8;
    }
    
    LoadImage(&rect, pImageBuffer);
    DrawSync(0);
    HeapFree(pImageBuffer);
}

// Zoom fade effect stuff
void func_800A5884(void) {
    int i;
    int nCurX;

    FieldZoomFadeEffectInitialize();
    for (i = 0; i < 2; i++) {
        FieldClearAndSwapOTag();
        FieldZoomFadeEffectUpdate();
        FieldDisplay();
    }

    nCurX= 0x2C0;
    for (i = 0; i < 5; i++) {
        func_800A5774(nCurX, 0x100, 0xE0);
        nCurX += 0x40;
    }

    for (i = 0; i < 2; i++) {
        FieldClearAndSwapOTag();
        FieldZoomFadeEffectUpdate();
        FieldDisplay();
    }
}

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A5924);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A5C40);

// Or Draw?
INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", FieldZoomFadeEffectUpdate);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", FieldZoomFadeEffectInitialize);

void FieldDisplay(void) {
    DrawSync(0);
    Vsync(2);
    ClearImage(&g_FieldCurRenderContext->drawEnvs[0].clip, 0x0, 0x0, 0x0);
    PutDrawEnv(&g_FieldCurRenderContext->drawEnvs[0]);
    PutDispEnv(&g_FieldCurRenderContext->dispEnv);
    DrawOTag(g_FieldCurRenderContext->ot3 + 7);
}

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A6998);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A6C40);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A6E70);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A7064);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A708C);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A7120);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A7218);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A732C);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A7394);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A73E8);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A74F8);

extern int g_FieldPixelIndex;
extern u_long g_FieldCurPixel;
extern u_long* g_Field24BitImageData;
extern u_long* g_Field15BitImageData;

u_int FieldImageConvert24BitColorTo15Bit(void) {
    u_long nPixel;
    u_int nLSB;

    // Are we dong reading RGB channels?
    if (!(g_FieldPixelIndex & 3)) {
        nPixel = *g_Field24BitImageData;
        g_Field24BitImageData += 1;
        g_FieldCurPixel = nPixel;
    }
    
    g_FieldPixelIndex++;
    
    nLSB = g_FieldCurPixel & 0xFF;
    g_FieldCurPixel >>= 8;
    
    if (nLSB) {
        nLSB = nLSB >> 3;
        if (nLSB == 0)
            nLSB = 1;
    }
    
    return nLSB;
}

void FieldImageConvert24BitTo15Bit(void) {
    RECT rect;
    int i;
    int j;
    u_long* pImage24Bit;
    u_long* pImage15Bit;
    u_long n15BitPixels;

    pImage24Bit = HeapAlloc(0xA800, 0x0);
    pImage15Bit = HeapAlloc(0x7000, 0x0);
    
    for (i = 0; i < 5; i++) {
        rect.x = i * 0x60;
        rect.y = 0;
        rect.w = 0x60;
        rect.h = 0xE0;
        StoreImage(&rect, pImage24Bit);
        DrawSync(0);
        g_Field24BitImageData = pImage24Bit;
        g_Field15BitImageData = pImage15Bit;
        g_FieldPixelIndex = 0;
        
        for (j = 0; j < 0x1C00; j++) {
            // 15 Bit: RGBRGB
            n15BitPixels = FieldImageConvert24BitColorTo15Bit();
            n15BitPixels |= FieldImageConvert24BitColorTo15Bit() << 0x5;
            n15BitPixels |= FieldImageConvert24BitColorTo15Bit() << 0xA;
            n15BitPixels |= FieldImageConvert24BitColorTo15Bit() << 0x10;
            n15BitPixels |= FieldImageConvert24BitColorTo15Bit() << 0x15;
            n15BitPixels |= FieldImageConvert24BitColorTo15Bit() << 0x1A;
            *g_Field15BitImageData = n15BitPixels;
            g_Field15BitImageData += 1;
        }
        
        rect.x = i << 6;
        rect.y = 0x100;
        rect.w = 0x40;
        rect.h = 0xE0;
        LoadImage(&rect, pImage15Bit);
        DrawSync(0);
    }
    HeapFree(pImage24Bit);
    HeapFree(pImage15Bit);
}

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A7948);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A7C58);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A8314);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A83B4);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A8408);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A84C0);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A8BA4);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A8EAC);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A90B4);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A915C);
/*
extern int D_800ADB34;
extern RECT g_FieldStoredImageDest;
extern u_long* D_800AFC70;

void func_800A915C(void) {
    if (D_800ADB34 != 1) {
        D_800ADB34 = 1;
        HeapChangeCurrentUser(HEAP_USER_YOSI, NULL);
        D_800AFC70 = HeapAlloc(0x8000, 0x1);
        g_FieldStoredImageDest.x = 0x3C0;
        g_FieldStoredImageDest.y = 0x100;
        g_FieldStoredImageDest.w = 0x40;
        g_FieldStoredImageDest.h = 0x100;
        StoreImage(&g_FieldStoredImageDest, D_800AFC70);
        DrawSync(0);
    }
}
*/


INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A91F0);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A9274);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A92AC);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A9374);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A93CC);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A9460);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A94A4);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A9688);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A987C);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A98B4);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A98E8);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A99A8);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A9B1C);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A9B54);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800A9F18);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800AA6B4);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800AA9DC);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800AAA74);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800AABD8);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800AAC08);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800AADC8);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800AAE4C);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800AAF80);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800AB328);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800AB378);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800AB748);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800AB808);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800ABA98);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800ABD18);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800ABEC8);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800ABFDC);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800AC03C);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800AC0F0);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800AC308);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800AC3AC);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800AC99C);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800ACB90);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800ACC58);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800ACCB0);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800ACCF4);

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800ACD7C);

u_short FieldScriptVMGetInstructionArgument(int argumentIndex) {
    u_char* pData;

    pData = g_FieldScriptVMCurScriptData + (g_FieldScriptVMCurActor->scriptInstructionPointer + argumentIndex);
    return *pData | *(pData + 1) <<  8;
}

INCLUDE_ASM("asm/field/nonmatchings/scripts/virtual_machine", func_800ACDEC);
