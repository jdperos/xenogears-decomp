#include "common.h"

#include "libspu_internal.h"

SpuTransferCallbackProc SpuSetTransferCallback(SpuTransferCallbackProc func) {
    SpuTransferCallbackProc previousCallback = _spu_transferCallback;
    if (func != previousCallback) {
        _spu_transferCallback = func;
    }
    return previousCallback;
}