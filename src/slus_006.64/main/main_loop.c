#include "common.h"
#include "main/main.h"
#include "system/memory.h"

extern unsigned int g_CurGameState;
extern unsigned int g_CurGameStateOverlayID;
extern void* g_CurGameStateOverlayBuffer;

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