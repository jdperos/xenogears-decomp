#include "common.h"

#include "libspu_internal.h"

long SpuInitMalloc(long num, char *top) {
    if (num > 0) {
        ((SPU_MALLOC *)top)->addr = SPU_MALLOC_END_MARKER | SPU_RESERVED_TOTAL;
        ((SPU_MALLOC *)top)->size = (_SPU_RAM_SIZE << _spu_mem_mode_plus) - SPU_RESERVED_TOTAL;
        _spu_memList = top;
        _spu_AllocLastNum = 0;
        _spu_AllocBlockNum = num;
        return num;
    }

    return 0;
}