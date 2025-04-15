#ifndef _MEMORY_H
#define _MEMORY_H


#define HEAP_NUM_USERS 0xA

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
#define HEAP_USER_UNKNOWN 0xA
#define HEAP_USER_TEST 0xB

// Block content types 0x1 -> 0x1F can be custom-defined
// on a per-user basis.
#define HEAP_DEFAULT_CONTENT_TYPES 0x20
#define HEAP_CONTENT_NULL 0x0

// 0x80050140 - Default Block content types
#define HEAP_CONTENT_NONE (HEAP_DEFAULT_CONTENT_TYPES | 0x0)
#define HEAP_CONTENT_FREE (HEAP_DEFAULT_CONTENT_TYPES | 0x1)
#define HEAP_CONTENT_FAKE_MALLOC (HEAP_DEFAULT_CONTENT_TYPES | 0x2)
#define HEAP_CONTENT_FAKE_CALLOC (HEAP_DEFAULT_CONTENT_TYPES | 0x3)
#define HEAP_CONTENT_MODEL_DATA (HEAP_DEFAULT_CONTENT_TYPES | 0x4)
#define HEAP_CONTENT_MODEL_PACKET (HEAP_DEFAULT_CONTENT_TYPES | 0x5)
#define HEAP_CONTENT_MODEL_LIGHT (HEAP_DEFAULT_CONTENT_TYPES | 0x6)
#define HEAP_CONTENT_CD_CHACE (HEAP_DEFAULT_CONTENT_TYPES | 0x7)
#define HEAP_CONTENT_MES_IMAGE (HEAP_DEFAULT_CONTENT_TYPES | 0x8)
#define HEAP_CONTENT_MES_WORK (HEAP_DEFAULT_CONTENT_TYPES | 0x9)
#define HEAP_CONTENT_MES_CUE (HEAP_DEFAULT_CONTENT_TYPES | 0xA)
#define HEAP_CONTENT_MIME_WORK (HEAP_DEFAULT_CONTENT_TYPES | 0xB)
#define HEAP_CONTENT_MIME_VERTEX (HEAP_DEFAULT_CONTENT_TYPES | 0xC)
#define HEAP_CONTENT_MIME_NORMAL (HEAP_DEFAULT_CONTENT_TYPES | 0xD)
#define HEAP_CONTENT_SYMBOL_DATA (HEAP_DEFAULT_CONTENT_TYPES | 0xE)
#define HEAP_CONTENT_SOUND (HEAP_DEFAULT_CONTENT_TYPES | 0xF)
#define HEAP_CONTENT_MES_FONT (HEAP_DEFAULT_CONTENT_TYPES | 0x10)
#define HEAP_CONTENT_MES_SYSDATA (HEAP_DEFAULT_CONTENT_TYPES | 0x11)
#define HEAP_CONTENT_LS_FONT (HEAP_DEFAULT_CONTENT_TYPES | 0x12)
#define HEAP_CONTENT_DELAY_FREE (HEAP_DEFAULT_CONTENT_TYPES | 0x13)

// Error handler states
#define HEAP_ERROR_HANDLER_ON 0x0
#define HEAP_ERROR_HANDLER_OFF 0x1

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
#define HEAP_DEBUG_PRINT_ALL 0xFFFFFFFF

typedef struct {
    void* pNext;
    u32 sourceAddress: 21;
    u32 userTag: 4;
    u32 isPinned: 1;
    u32 contentTag: 6;
} HeapBlock;

struct HeapDelayedFreeBlock {
    struct HeapDelayedFreeBlock* pNext;
    void* pMem;
    u32 delay;
};

typedef struct HeapDelayedFreeBlock HeapDelayedFreeBlock;

extern HeapDelayedFreeBlock g_HeapDelayedFreeBlocksHead;

extern u32 __attribute__((noreturn)) func_80019ACC(u32);
extern void func_80031FF8();

extern unsigned int StrLen(char* pString);
extern void HeapPrintf(char*, ...);

extern char** D_80050110;
extern char** g_HeapContentTypeNames[];

// Pointers to Content type name arrays for specific heap users
extern char** g_HeapUserContentNames[HEAP_NUM_USERS];


int HeapLoadSymbols(char* pSymbolFilePath);
void HeapInit(void* heapStart, void* heapEnd);
void HeapRelocate(void* pNewStartAddress);
u16 HeapGetCurrentUser(void);
void HeapSetCurrentUser(u16 userTag);
unsigned int HeapToggleErrorHandler(unsigned int status);
void HeapGetAllocInformation(u32* pAllocSourceAddr, u32* pAllocSize);
void* HeapAlloc(u32 allocSize, u32 allocFlags);
void* HeapInsertAlloc(HeapBlock* pMem, u32 allocSize);
void HeapConsolidate(void);
void HeapPinBlock(HeapBlock* pBlock);
void HeapUnpinBlock(HeapBlock* pBlock);
void HeapUnpinBlockCopy(HeapBlock* pBlock);
u32 HeapFree(void* pMem);
void HeapFreeBlocksWithFlag(u8 targetFlag);
void HeapFreeAllBlocks(void);
void HeapForceFreeAllBlocks(void);
u32 HeapGetTotalFreeSize(void);
u32 HeapWalkUntilEnd(void);
u32 HeapGetLargestFreeBlockSize(void);
void HeapChangeCurrentUser(u32 userTag, char** pContentTypes);
void HeapSetCurrentContentType(u16 contentTag);
void HeapGetSymbolNameFromAddress(u32 address, u8* pString);
void HeapDebugDumpBlock(HeapBlock* pBlockHeader, void* pBlockMem, u32 blockSize, s32 debugFlags);
void HeapDebugDump(u32 mode, u32 startBlockIdx, s32 endBlockIdx, u32 flags);
void* HeapAllocSound(u32 allocSize);
void HeapCalloc(u32 numElements, u32 elementSize);
void HeapForceFree(void* pMem);
void HeapDelayedFree(void* pMem, u32 delay);
void HeapTickDelayedFree(void);
void HeapFreeAllDelayedBlocks(void);
void HeapWriteToDebugFile(char* pBuffer);
void HeapDumpToFile(char *pOutputFilePath);
#endif