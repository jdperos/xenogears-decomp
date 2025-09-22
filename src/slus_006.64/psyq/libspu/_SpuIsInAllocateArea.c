#include "common.h"

#include "libspu_internal.h"

long _SpuIsInAllocateArea(u_long addr) {
    SPU_MALLOC* mem_entry;
    long index;
    SPU_MALLOC* mem_list = (SPU_MALLOC*)_spu_memList;

    if (_spu_memList == NULL) {
        return 0;
    }

    for (index = 0; 1; index++) {
        mem_entry = &mem_list[index];

        if (mem_entry->addr & SPU_MALLOC_SKIP_ENTRY) continue;

        if (mem_entry->addr & SPU_MALLOC_END_MARKER) break;

        if ((mem_entry->addr & SPU_MALLOC_ADDR_MASK) >= addr) {
            return 1;
        }

        if (((mem_entry->addr & SPU_MALLOC_ADDR_MASK) + mem_entry->size) > addr) {
            return 1;
        }
    }

    return 0;
}

// The same as _SpuIsInAllocateArea, but with an SPU-ranged memory address
long _SpuIsInAllocateArea_(long addr) {
    SPU_MALLOC* mem_entry;
    long index;
    SPU_MALLOC* mem_list = (SPU_MALLOC*)_spu_memList;

    addr <<= _spu_mem_mode_plus;

    if (_spu_memList == NULL) {
        return 0;
    }

    for (index = 0; 1; index++) {
        mem_entry = &mem_list[index];

        if (mem_entry->addr & SPU_MALLOC_SKIP_ENTRY) continue;

        if (mem_entry->addr & SPU_MALLOC_END_MARKER) break;

        if ((mem_entry->addr & SPU_MALLOC_ADDR_MASK) >= addr) {
            return 1;
        }

        if (((mem_entry->addr & SPU_MALLOC_ADDR_MASK) + mem_entry->size) > addr) {
            return 1;
        }
    }

    return 0;
}