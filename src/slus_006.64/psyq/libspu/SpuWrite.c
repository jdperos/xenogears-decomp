#include "common.h"

#include "libspu_internal.h"

u_long SpuWrite(u_char *addr, u_long size) {
    if (size > SPU_MAX_TRANSFER_SIZE) {
        size = SPU_MAX_TRANSFER_SIZE;
    }

    _spu_Fw(addr, size);
    if (_spu_transferCallback == NULL) {
        _spu_inTransfer = 0;
    }

    return size;
}