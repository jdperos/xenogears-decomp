#include "common.h"

#include "libspu_internal.h"

// TODO(jperos): The significance of these constants is yet not understood by me
long SpuClearReverbWorkArea (long mode) {
    volatile s32 callback;
    s32 oldTransmode;
    s32 addr;
    s32 bTransferring;
    s32 bTansmodeCleared;
    u32 size;
    u32 remaining;

    callback = 0;
    bTansmodeCleared = 0;

    if ((mode >= (u_long)SPU_REV_MODE_MAX) ||
        (_SpuIsInAllocateArea_(_spu_rev_startaddr[mode]))) {
        return SPU_ERROR;
        }

    if (mode == SPU_REV_MODE_OFF) {
        remaining = 0x10 << _spu_mem_mode_plus;
        addr = 0xFFF0 << _spu_mem_mode_plus;
    } else {
        remaining = (_SPU_RAM_SIZE - _spu_rev_startaddr[mode]) << _spu_mem_mode_plus;
        addr = _spu_rev_startaddr[mode] << _spu_mem_mode_plus;
    }

    oldTransmode = _spu_transMode;
    if (_spu_transMode == 1) {
        _spu_transMode = 0;
        bTansmodeCleared = 1;
    }

    bTransferring = 1;
    if (_spu_transferCallback != 0) {
        callback = _spu_transferCallback;
        _spu_transferCallback = 0;
    }

    while (bTransferring) {
        size = remaining;
        if (!(remaining < (0x400U + 1))) {
            size = 0x400;
        } else {
            bTransferring = 0;
        }
        _spu_t(SPU_DMA_CMD_SETADDR, addr);
        _spu_t(SPU_DMA_CMD_WRITE);
        _spu_t(SPU_DMA_CMD_EXEC, &_spu_zerobuf, size);
        WaitEvent(_spu_EVdma);
        remaining -= 0x400;
        addr += 0x400;
    };

    if (bTansmodeCleared) {
        _spu_transMode = oldTransmode;
    }

    if (callback) {
        _spu_transferCallback = callback;
    }

    return SPU_SUCCESS;
}