#include "common.h"

#include "libspu_internal.h"

u_long SpuSetTransferStartAddr(u_long addr) {
    u32 offset;
    u16 base_addr;

    offset = addr - SPU_MIN_ADDR;
    if (offset > SPU_MAX_VALID_OFFSET) {
        return 0;
    }

    base_addr = _spu_FsetRXXa(SPU_ADDRESS_MODE_SPU, addr);
    _spu_tsa = base_addr;
    return (ulong)base_addr << _spu_mem_mode_plus;
}