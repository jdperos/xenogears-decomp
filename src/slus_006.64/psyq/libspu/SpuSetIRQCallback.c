#include "common.h"

#include "libspu_internal.h"

SpuIRQCallbackProc SpuSetIRQCallback(SpuIRQCallbackProc func) {
    SpuIRQCallbackProc callback = _spu_IRQCallback;
    if (func != callback ) {
        _spu_IRQCallback = func;
        _SpuCallback(func);
    }
    return callback;
}