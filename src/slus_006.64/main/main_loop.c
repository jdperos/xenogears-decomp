#include "common.h"
#include "main/main.h"
#include "system/memory.h"
#include "system/controller.h"

#include "psyq/libetc.h"
#include "psyq/libgpu.h"
#include "psyq/pc.h"

extern unsigned int g_CurGameState;
extern unsigned int g_CurGameStateOverlayID;
extern void* g_CurGameStateOverlayBuffer;

extern void* LZSSHeapDecompress(void*, int flags); 
extern u8 g_PublishedByLogoCompressed[];

extern void func_800295D8(s32, void*, u32, u32);


void ChangeGameState(unsigned int state) {
    g_CurGameState = state;
    if (state != g_CurGameStateOverlayID) {
        if (g_CurGameStateOverlayBuffer) {
            HeapFree(g_CurGameStateOverlayBuffer);
            g_CurGameStateOverlayBuffer = NULL;
        }
        g_CurGameStateOverlayID = -1;
    }
}

void* LoadGameStateOverlay(unsigned int overlayIndex) {
    int nPrevSectionIndex;
    int nPrevEntryIndex;
    unsigned int nPrevHeapUser;
    unsigned int nPrevHeapErrorHandlerStatus;
    unsigned int nFileSize;
    void* pBuffer;
    
    if (g_CurGameStateOverlayID != overlayIndex) {
        g_CurGameStateOverlayID = overlayIndex;
        
        nPrevHeapUser = HeapGetCurrentUser();
        ArchiveGetArchiveOffsetIndices(&nPrevSectionIndex, &nPrevEntryIndex);
        HeapSetCurrentUser(HEAP_USER_SUGI);
        ArchiveSetIndex(0x0, 0x1);
        nPrevHeapErrorHandlerStatus = HeapToggleErrorHandler(0x1);

        nFileSize = ArchiveDecodeSize(g_GameStateOverlayArchiveOffsets[overlayIndex]);
        pBuffer = HeapAlloc(nFileSize, 0x1);
        g_CurGameStateOverlayBuffer = pBuffer;
        if (pBuffer != NULL) {
            // Read archive file into buffer
            func_800295D8(g_GameStateOverlayArchiveOffsets[overlayIndex], pBuffer, 0, 0);
        } else {
            g_CurGameStateOverlayID = -1;
        }

        HeapToggleErrorHandler(nPrevHeapErrorHandlerStatus);
        ArchiveSetIndex(nPrevSectionIndex, nPrevEntryIndex);
        HeapSetCurrentUser(nPrevHeapUser);
    }

    return g_CurGameStateOverlayBuffer;
}

void MainLoop(int errorCode) {
    char sp[8];
    unsigned int nCallerAddr;
    MainGameState* pGameState;
    void* pOverlayData;

    // Error handler
    if (errorCode) {
        asm volatile(
            "move $t7, %0\n\t"
            "sw $ra, 0($t7)\n\t"
        :: "r"(&nCallerAddr));
        GameHandleError(errorCode, nCallerAddr);
    }
    
    pGameState = &g_MainGameStates[g_CurGameState];
    ResetGraph(1);
    DrawSyncCallback(NULL);
    func_800363F0(0);
    DrawSync(0);
    Vsync(2);
    HeapRelocate(pGameState->pHeapStart + 0x800);
    HeapResetUser();
    
    if (pGameState->hasOverlay) {
        ClearMemory(pGameState->pMemStart, pGameState->pHeapStart);
        pOverlayData = LoadGameStateOverlay(g_CurGameState);
        ArchiveCdDataSync(0);

        // Decompress overlay into global overlay buffer
        LZSSDecompress(pOverlayData, g_MainGameStateOverlayBuffer);
        
        DrawSync(0);
        Vsync(0);
        EnterCriticalSection();
        DrawSync(0);
        Vsync(0);
        FlushCache();
        ExitCriticalSection();
    }
    
    func_80019548(); // Does nothing, likely a debug function
    HeapRelocate(pGameState->pHeapStart + 4);
    HeapReset();
    ControllerResetState();
    ChangeGameState(0);
    pGameState->pFnMain();
    MainLoop(0);
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/main/main_loop", func_80019C2C);
/*
void func_80019C2C(void) {
  int hFile;
  
  PCinit();
  hFile = PCcreate("c:\\core", 0);
  PCwrite(hFile, 0x80000000, 0x200000);
  PCclose(hFile);
}
*/

void HeapResetUser() {
    HeapChangeCurrentUser(HEAP_USER_UNKNOWN, 0);
}

void GameCheckAndHandleSoftReset(void) {
    if (g_C1ButtonState == (CTRL_BTN_START | CTRL_BTN_SELECT | CTRL_BTN_R1 | CTRL_BTN_L1)) {
        GameSoftReset();
    }
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/main/main_loop", GameSoftReset);

void GameShowSplashScreen(void) {
    DRAWENV drawEnv;
    DISPENV dispEnv;
    SPRT sprite;
    RECT rect;
    int i;
    void* pClutData;
    void* pImage;

    HeapSetCurrentUser(HEAP_USER_SUGI);
    pImage = LZSSHeapDecompress(&g_PublishedByLogoCompressed, 1);

    // Load CLUT
    pClutData = pImage + 0x14;
    rect.y = 0xF0;
    rect.w = 0x10;
    rect.x = 0;
    rect.h = 1;
    LoadImage(&rect, pClutData);

    // Load Image Data
    rect.x = 0x280;
    rect.y = 0;
    rect.w = 0x40;
    rect.h = 0x30;
    LoadImage(&rect, pImage + 0x40);
    
    setSprt(&sprite);
    setXY0(&sprite, 0x20, 0x58);
    sprite.v0 = 0;
    sprite.u0 = 0;
    setWH(&sprite, 0x100, 0x30);
    setClut(&sprite, 0x0, 0xF0);
    
    SetDefDrawEnv(&drawEnv, 0, 0, 0x140, 0xE0);
    SetDefDispEnv(&dispEnv, 0, 0, 0x140, 0xE0);
    PutDrawEnv(&drawEnv);
    PutDispEnv(&dispEnv);
    DrawSync(0);
    
    for (i = 0; i < 0x80; i += 8) {
        setRGB0(&sprite, i, i, i);
        DrawPrim(&sprite);
        Vsync(0);
    }

    for (i = 0x6D; i != -1; i--) {
        Vsync(0);
    };

    for (i = 0x80; i >= 0; i -= 8) {
        setRGB0(&sprite, i, i, i);
        DrawPrim(&sprite);
        Vsync(0);
    };
    
    HeapFree(pImage);
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/main/main_loop", GameHandleError);