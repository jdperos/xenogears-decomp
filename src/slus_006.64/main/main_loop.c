#include "common.h"

extern unsigned int g_CurGameState;
extern unsigned int g_CurOverlayID;
extern void* g_CurOverlayBuffer;

void ChangeGameState(unsigned int state) {
    g_CurGameState = state;
    if (state != g_CurOverlayID) {
        if (g_CurOverlayBuffer != 0) {
            HeapFree(g_CurOverlayBuffer);
            g_CurOverlayBuffer = 0;
        }
        g_CurOverlayID = -1;
    }
}
