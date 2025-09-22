#include "common.h"

#include "libspu_internal.h"

long SpuSetTransferMode(long mode) {
    int value;

    switch (mode) {
        case SPU_TRANSFER_BY_DMA:
            value = 0;
            break;
        case SPU_TRANSFER_BY_IO:
            value = 1;
            break;
        default:
            value = 0;
    }
    _spu_trans_mode = mode;
    _spu_transMode = value;
    return value;
}