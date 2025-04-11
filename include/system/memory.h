#ifndef _MEMORY_H
#define _MEMORY_H


extern s32 D_80059FCC[2];

extern u32 __attribute__((noreturn)) func_80019ACC(u32);
extern void func_80031FF8();

#define HEAP_BLOCK_FREE 0
#define HEAP_BLOCK_END 1

typedef struct {
    void* pNext;
    u32 flagAllocSrc: 21;
    u32 flagUnk: 4;
    u32 flagPinned: 1;
    u32 flagMemType: 6;
} HeapBlock;

void InitHeap(void* heapStart, void* heapEnd);

#endif