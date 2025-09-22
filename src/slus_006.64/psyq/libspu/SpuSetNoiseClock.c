#include "common.h"

#include "libspu_internal.h"

long SpuSetNoiseClock(long n_clock) {
    long clamped;
    u16 spucnt;
    u16 newVal;


    if (n_clock < 0) {
        clamped = 0;
    } else if (n_clock > 0x3F) {  // >= 64
        clamped = 0x3F;      // = 63
    } else {
        clamped = n_clock;
    }

    spucnt = _spu_RXX->_rxx.spucnt;
    newVal = (spucnt & 0xC0FF) | ((clamped & 0x3F) << 8);
    _spu_RXX->rxx.spucnt = newVal;

    return clamped;
}