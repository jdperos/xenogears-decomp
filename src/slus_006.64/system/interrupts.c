#include "common.h"

extern void* g_VsyncInterruptCallbacks[8];

void SetVsyncInterruptCallbacksByIndex(u32 index, void* fnCallback) {
    if (fnCallback != g_VsyncInterruptCallbacks[index]) {
        g_VsyncInterruptCallbacks[index] = fnCallback;
    }
}

void ClearVsyncInterruptCallbacks(void** callbacks, unsigned int numCallbacks) {
    while (numCallbacks-- != 0) {
        *callbacks++ = NULL;
    }
}