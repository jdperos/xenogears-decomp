#include "common.h"
#include "system/memory.h"


u16 D_80059318;
u16 D_8005931C;
void* g_Heap;
u32 g_HeapNeedsConsolidation;
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
    g_HeapNeedsConsolidation = 0;
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

void HeapGetAllocInformation(u32* pAllocSourceAddr, u32* pAllocSize) {
    *pAllocSourceAddr = D_80059340;
    *pAllocSize = D_8005933C;
}

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
    
    if (g_HeapNeedsConsolidation != 0) {
        HeapConsolidate();
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
    
    while (1) {
        while (pCurBlockHeader->flagUnk != HEAP_BLOCK_FREE) {
            if (pCurBlockHeader->flagUnk == HEAP_BLOCK_END) {
                goto l_0x1CC_EndOfHeap;
            }

            pCurBlock = pCurBlockHeader->pNext;
            pCurBlockHeader = pCurBlock - 1;
        }
        
        nFreeSize = (u32)pCurBlockHeader->pNext - (u32)pCurBlockHeader - 0x10;
        nRemainingSize = nFreeSize - allocSize;

        if (nRemainingSize == 4 || nRemainingSize == 0) {
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
        } else if (nRemainingSize >= 5) {
            bOutOfMemory = 0;
    
            if (allocFlags != 1) {
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
        if (D_80059330 != 0) {
            return 0;
        }

        // Goes into error handler, does not return.
        func_80019ACC(ERR_HEAP_OUT_OF_MEMORY);
    }

    if (allocFlags == 1) {
        // Small block allocation
        if (pFreeBlockHeader < pSmallBlock) {
            pCurBlockHeader = pSmallBlock;
            goto l_0xFC_AllocSmallBlock;
        }

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

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/memory", func_80031F70);

void HeapConsolidate(void) {
    HeapBlock* pCurrent;
    HeapBlock* pOther;
    
    pCurrent = (HeapBlock*)g_Heap - 1;

    while (pCurrent->flagUnk != HEAP_BLOCK_END) {
        if (pCurrent->flagUnk == HEAP_BLOCK_FREE) {
            pOther = pCurrent->pNext;
            while (
                (pCurrent->flagUnk == HEAP_BLOCK_FREE) &&
                (pOther[-1].flagUnk == HEAP_BLOCK_FREE)
            ) {
                pOther = pOther[-1].pNext;
                pCurrent->pNext = pOther;
            }
        }
        pCurrent = (HeapBlock*)pCurrent->pNext - 1;
    }
    
    g_HeapNeedsConsolidation = 0;
}

void HeapPinBlock(HeapBlock* pBlock) {
    pBlock[-1].flagPinned = 1;
}

void HeapUnpinBlock(HeapBlock* pBlock) {
    pBlock[-1].flagPinned = 0;
}

void HeapUnpinBlockCopy(HeapBlock* pBlock) {
    pBlock[-1].flagPinned = 0;
}

u32 HeapFree(void* pMem) {
    u32 nCallerAddr;
    HeapBlock* pBlock;  

    if (pMem == NULL) {
        if (D_80059330 != 0) {
            return 1;
        }
        
        asm volatile(
            "move $t7, %0\n\t"
            "sw $ra, 0($t7)\n\t"
        :: "r"(&nCallerAddr));
        D_8005933C = 0;
        D_80059340 = nCallerAddr - 8;
        func_80019ACC(ERR_HEAP_FREE_NULL);
    }

    pBlock = ((HeapBlock*)pMem);
    if (pBlock[-1].flagPinned == 0) {
        pBlock[-1].flagMemType = 0x21;
        pBlock[-1].flagUnk = 0x0;
        pBlock[-1].flagAllocSrc = 0;
        g_HeapNeedsConsolidation = 1;
        return 0;
    } else if (pBlock[-1].flagPinned == 1) {
        return -1;
    }
}

void HeapFreeBlocksWithFlag(u8 targetFlag) {
    void* pMem;
    HeapBlock* pCurBlock;

    pCurBlock = (HeapBlock*)g_Heap - 1;
    while (pCurBlock->flagUnk != HEAP_BLOCK_END) {
        if (pCurBlock->flagUnk == targetFlag) {
            pMem = pCurBlock;
            pCurBlock = (HeapBlock*)pCurBlock->pNext - 1;
            HeapFree(pMem + sizeof(HeapBlock));
        } else {
            pCurBlock = (HeapBlock*)pCurBlock->pNext - 1;
        }
    }
}