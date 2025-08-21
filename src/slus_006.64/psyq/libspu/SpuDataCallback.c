#include "common.h"

#include "libspu_internal.h"

void _SpuDataCallback(SpuTransferCallbackProc func) {
    DMACallback(4, func);
}