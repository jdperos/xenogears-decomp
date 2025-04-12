#ifndef _MEMORY_H
#define _MEMORY_H


extern s32 D_80059FCC[3];

extern u32 __attribute__((noreturn)) func_80019ACC(u32);
extern void func_80031FF8();

extern void func_80032BDC(char*, ...);

extern char** D_80050110;
extern char** D_80050140;
extern char** D_80059FA4;


// 0x80050110 - Block user flags
#define HEAP_USER_NONE 0x0
#define HEAP_USER_END 0x1
#define HEAP_USER_HIG 0x2
#define HEAP_USER_KAZM 0x3
#define HEAP_USER_MASA 0x4
#define HEAP_USER_MIYA 0x5
#define HEAP_USER_SUGI 0x6
#define HEAP_USER_SUZU 0x7
#define HEAP_USER_YOSI 0x8
#define HEAP_USER_SIMA 0x9
#define HEAP_USER_UNKNOWN 0x10
#define HEAP_USER_TEST 0x11

// 0x80050140 - Block content flags
#define HEAP_CONTENT_NONE 0x0
#define HEAP_CONTENT_FREE 0x1
#define HEAP_CONTENT_FAKE_MALLOC 0x2
#define HEAP_CONTENT_FAKE_CALLOC 0x3
#define HEAP_CONTENT_MODEL_DATA 0x4
#define HEAP_CONTENT_MODEL_PACKET 0x5
#define HEAP_CONTENT_MODEL_LIGHT 0x6
#define HEAP_CONTENT_CD_CHACE 0x7
#define HEAP_CONTENT_MES_IMAGE 0x8
#define HEAP_CONTENT_MES_WORK 0x9
#define HEAP_CONTENT_MES_CUE 0x10
#define HEAP_CONTENT_MIME_WORK 0x11
#define HEAP_CONTENT_MIME_VERTEX 0x12
#define HEAP_CONTENT_MIME_NORMAL 0x13
#define HEAP_CONTENT_SYMBOL_DATA 0x14
#define HEAP_CONTENT_SOUND 0x15
#define HEAP_CONTENT_MES_FONT 0x16
#define HEAP_CONTENT_MES_SYSDATA 0x17
#define HEAP_CONTENT_LS_FONT 0x18
#define HEAP_CONTENT_DELAY_FREE 0x19

// Error flags
#define ERR_HEAP_OUT_OF_MEMORY 0x82
#define ERR_HEAP_FREE_NULL 0x83

// Debug flags
#define HEAP_DEBUG_PRINT_NO 0x1
#define HEAP_DEBUG_PRINT_MCB 0x2
#define HEAP_DEBUG_PRINT_ADDRESS 0x4
#define HEAP_DEBUG_PRINT_SIZE 0x8
#define HEAP_DEBUG_PRINT_USER 0x10
#define HEAP_DEBUG_PRINT_GETADD 0x20
#define HEAP_DEBUG_PRINT_FUNCTION 0x40
#define HEAP_DEBUG_PRINT_CONTENTS 0x80
#define HEAP_DEBUG_PRINT_TOTAL_FREE_SIZE 0x8000

typedef struct {
    void* pNext;
    u32 sourceAddress: 21;
    u32 userTag: 4;
    u32 isPinned: 1;
    u32 contentTag: 6;
} HeapBlock;

void HeapInit(void* heapStart, void* heapEnd);

#endif