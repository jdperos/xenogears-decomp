#include "common.h"

#include "libspu_internal.h"

void _SpuCallback(SpuIRQCallbackProc func) {
    InterruptCallback(9, func);
}