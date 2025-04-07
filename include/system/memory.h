#ifndef _MEMORY_H
#define _MEMORY_H

extern s16 D_80059318;
extern s16 D_8005931C;
extern void* pHeap;
extern s32 D_80059324;
extern s32 D_80059328;
extern s32 D_8005932C;
extern s32 D_80059330;
extern s32 D_80059334;
extern s32 D_80059338;
extern s32 D_80059FCC[2];

typedef struct {
    void* pNext;
    u32 flagAllocSrc: 21;
    u32 flagUnk: 4;
    u32 flagPinned: 1;
    u32 flagMemType: 6;
} HeapBlock;

void InitHeap(void* heapStart, void* heapEnd);

#endif