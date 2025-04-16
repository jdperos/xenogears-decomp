#include "common.h"

#include "system/interrupts.h"

void SetVsyncInterruptCallbackByIndex(u32 index, VoidCallback_t callback) {
    if (callback != g_VsyncInterruptCallbacks[index]) {
        g_VsyncInterruptCallbacks[index] = callback;
    }
}

void ClearVsyncInterruptCallbacks(void(**callbacks)(void), unsigned int numCallbacks) {
    while (numCallbacks--) {
        *callbacks++ = NULL;
    }
}

/*
Matches on decomp.me, but the volatile globals seems to cause issues when linking.
*/
/*
void VsyncInterruptHandler(void) {
    int i;

    g_VsyncInterruptCount++;
    for (i = 0; i < MAX_INTERRUPT_CALLBACKS; i++) {
        if (g_VsyncInterruptCallbacks[i] != NULL) {
            g_VsyncInterruptCallbacks[i]();
        }
    }
}

SetVsyncIntrCallback_t InitVsyncInterrupts() {
    *g_pTMR_HRETRACE_MODE = 0x107;
    g_VsyncInterruptCount = 0;
    ClearVsyncInterruptCallbacks(g_VsyncInterruptCallbacks, 8);
    SetInterruptHandler(0, VsyncInterruptHandler);
    return &SetVsyncInterruptCallbackByIndex;
}
*/