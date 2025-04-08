#include "common.h"
#include "system/memory.h"

s16 D_80059318;
s16 D_8005931C;
void* g_Heap;
s32 D_8005932C;
s32 D_80059330;
s32 D_80059334;
s32 D_80059338;

//INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/memory", func_80032B64);
//INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/memory", func_800318F8);

void InitHeap(void* heapStart, void* heapEnd) {
    HeapBlock* startBlock = (HeapBlock*)((u32)heapStart & -4);
    HeapBlock* endBlock = (HeapBlock*)((u32)heapEnd & -4);
    g_Heap = &startBlock[1];
    startBlock->pNext = endBlock;
    
    D_80059318 = 0x20;
    D_8005931C = 0xa;
    D_8005932C = 0;
    D_80059334 = 0;
    D_80059338 = 0;
    
    startBlock->flagUnk = 0x0;
    startBlock->flagMemType = 0x21;
    
    endBlock[-1].pNext = endBlock;
    endBlock[-1].flagUnk = 0x1;
    endBlock[-1].flagMemType = 0x20;

    D_80059FCC[0] = 0;
    func_80031A30();
}