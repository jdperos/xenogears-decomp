#include "common.h"

#include "libspu_internal.h"

void SpuQuit(void) {
    if (g_SpuRunning == 1) {
        g_SpuRunning = 0;
        EnterCriticalSection();
        _spu_transferCallback = NULL;
        _spu_IRQCallback = NULL;
        _SpuDataCallback(NULL);
        CloseEvent(_spu_EVdma);
        DisableEvent(_spu_EVdma);
        ExitCriticalSection();
    }
}