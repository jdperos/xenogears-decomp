#include "common.h"

#include "libspu_internal.h"

u_long SpuRead(u_char* addr, u_long size) {

    if (size > SPU_MAX_TRANSFER_SIZE) {
        size = SPU_MAX_TRANSFER_SIZE;
    }
    _spu_Fr(addr, size);
    if (_spu_transferCallback == 0) {
        _spu_inTransfer = 0;
    }
    return size;
}