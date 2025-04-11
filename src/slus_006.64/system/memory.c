#include "common.h"
#include "system/memory.h"


u16 D_80059318;
u16 D_8005931C;
void* g_Heap;
s32 D_8005932C;
s32 D_80059330;
s32 D_80059334;
s32 D_80059338;
u32 D_8005933C;
u32 D_80059340;


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

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/memory", func_80031B10);
INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/memory", func_80031B9C);
INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/memory", func_80031BA8);
INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/memory", func_80031BB4);
INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/memory", func_80031BC4);

void* HeapAlloc(u32 allocSize, u32 allocFlags) {
    u32 nCallerAddr;
    u32 nFreeSize;
    s32 nRemainingSize;
    u32 bOutOfMemory;
    u32 nMinFreeSize;
    HeapBlock* pSmallBlock;
    HeapBlock* pCurBlockHeader;
    HeapBlock* pFreeBlock;
    HeapBlock* pFreeBlockHeader;
    HeapBlock* pNewBlock;
    HeapBlock* pNewBlock2;
    HeapBlock* pCurBlock;

    // Save address the called HeapAlloc
    asm volatile(
        "move $t7, %0\n\t"
        "sw $ra, 0($t7)\n\t"
    :: "r"(&nCallerAddr));
    
    nCallerAddr -= 8;
    D_80059340 = nCallerAddr;
    nCallerAddr = ((nCallerAddr << 7) >> 9);
    
    // Do we need to free stuff?
    if (D_8005932C != 0) {
        func_80031FF8();
    }

    bOutOfMemory = 1; 
    D_8005933C = allocSize;
    allocSize = (allocSize + 3);
    allocSize &= -4;
    pFreeBlock = NULL;
    nMinFreeSize = 0x800000;
    pCurBlock = g_Heap;
    pSmallBlock = pFreeBlockHeader = NULL;
    pCurBlockHeader = pCurBlock - 1;
    
    // 0x7C
    while (1) {
        while (pCurBlockHeader->flagUnk != HEAP_BLOCK_FREE) {
            // 0x90
            if (pCurBlockHeader->flagUnk == HEAP_BLOCK_END) {
                goto l_0x1CC_EndOfHeap;
            }
    
            // 0xA4
            pCurBlock = pCurBlockHeader->pNext;
            pCurBlockHeader = pCurBlock - 1;
        }
        
        // 0xC0
        nFreeSize = (u32)pCurBlockHeader->pNext - (u32)pCurBlockHeader - 0x10;
        nRemainingSize = nFreeSize - allocSize;

        if (nRemainingSize == 4 || nRemainingSize == 0) {
            // 0xE8
            bOutOfMemory = 0;

            // Small block allocation
            if (allocFlags == 1) {
                pSmallBlock = pCurBlockHeader; 
            } else {
                l_0xFC_AllocSmallBlock:
                pCurBlockHeader->flagUnk = D_8005931C;
                pCurBlockHeader->flagMemType = D_80059318;
                pCurBlockHeader->flagPinned = 0;
                pCurBlockHeader->flagAllocSrc = nCallerAddr;
                D_80059318 = 0x20;
                return pCurBlockHeader + 1;
            }
        }
        // 0x178:
        else if (nRemainingSize >= 5) {
            // 0x180
            bOutOfMemory = 0;
    
            if (allocFlags != 1) {
                // 0x188
                if (allocFlags != 2) {
                    goto l_0x1B4;
                }

                // Have we found a smaller block?
                if (nFreeSize < nMinFreeSize) {
                    pFreeBlock = pCurBlock;
                    pFreeBlockHeader = pCurBlockHeader;
                    nMinFreeSize = nFreeSize;
                }
                 goto l_0x1C0;
            }
            // 0x1AC
            pFreeBlockHeader = pCurBlockHeader;
            goto l_0x1C0;
    
            l_0x1B4:
            pFreeBlock = pCurBlock;
            pFreeBlockHeader = pCurBlockHeader;
            goto l_0x28C;
        }

        l_0x1C0:
        pCurBlock = pCurBlockHeader->pNext;
        pCurBlockHeader = pCurBlock - 1;
    }

    l_0x1CC_EndOfHeap:
    if (bOutOfMemory != 0) {
        // 0x1D4
        if (D_80059330 != 0) {
            return 0;
        }

        // Goes into error handler, does not return.
        func_80019ACC(0x82);
    }

    // 0x1EC
    if (allocFlags == 1) {
        // 1F0

        // Small block allocation
        if (pFreeBlockHeader < pSmallBlock) {
            pCurBlockHeader = pSmallBlock;
            goto l_0xFC_AllocSmallBlock;
        }

        // 0x200
        pNewBlock = pFreeBlockHeader->pNext - (allocSize + 8);
        pNewBlock[-1].pNext = pFreeBlockHeader->pNext;
        pNewBlock[-1].flagUnk = D_8005931C;
        pNewBlock[-1].flagMemType = D_80059318;
        pNewBlock[-1].flagPinned = 0;
        pNewBlock[-1].flagAllocSrc = nCallerAddr;
        pFreeBlockHeader->pNext = pNewBlock;
        D_80059318 = 0x20;
        return pNewBlock;
    }
    
    l_0x28C:
    pNewBlock2 = (HeapBlock*)((u32)pFreeBlock + allocSize); 
    pNewBlock2->pNext = pFreeBlockHeader->pNext;
    pNewBlock2->flagUnk = pFreeBlockHeader->flagUnk;
    pNewBlock2->flagMemType = pFreeBlockHeader->flagMemType;
    pNewBlock2->flagPinned = pFreeBlockHeader->flagPinned;
    pNewBlock2->flagAllocSrc = pFreeBlockHeader->flagAllocSrc;
    
    pFreeBlockHeader->pNext = pNewBlock2 + 1;
    pFreeBlockHeader->flagMemType = D_80059318;
    D_80059318 = 0x20;
    pFreeBlockHeader->flagUnk = D_8005931C;
    pFreeBlockHeader->flagPinned = 0;
    pFreeBlockHeader->flagAllocSrc = nCallerAddr;
    return pFreeBlock;
}