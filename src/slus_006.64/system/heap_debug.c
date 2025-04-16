#include "common.h"
#include "system/memory.h"
#include "psyq/pc.h"

extern void func_800379C8(u8*);

// .sbss
int g_HeapDebugDumpFileHandle;

void HeapWriteToDebugFile(char* pBuffer) {
    unsigned int nLen;

    nLen = strlen(pBuffer);
    PCwrite(g_HeapDebugDumpFileHandle, pBuffer, nLen);
}

void HeapDumpToFile(char *pOutputFilePath) {
    PCinit();
    g_HeapDebugDumpFileHandle = PCcreate(pOutputFilePath, 0);
    g_HeapDebugPrintfFn = (FnPrintf_t*)HeapWriteToDebugFile;
    HeapDebugDump(1, 0, 0, HEAP_DEBUG_PRINT_ALL);
    g_HeapDebugPrintfFn = (FnPrintf_t*)func_800379C8;
    PCclose(g_HeapDebugDumpFileHandle);
}

void* HeapDerefPtr(u32* pData) {
    return *pData;
}