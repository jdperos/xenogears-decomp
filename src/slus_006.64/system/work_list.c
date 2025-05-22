#include "common.h"
#include "system/memory.h"

/* A lot of this TU has been matched already, but due to SBSS/SDATA symbols
 * issues it's currently unable to be compiled in. */

// TU is compiled on GCC 2.7.2 or 2.6.0

typedef void (*WorkListCallback_t)(void*);

typedef struct {
    struct WorkListEntry* unk0; // 
    void* unk4; // pSpriteData, at least in some cases. Could be a more general pointer to data
    WorkListCallback_t onTriggerCallback;
    WorkListCallback_t onFreeCallback;

    // Flags, shrug
    u32 unk10: 29;
    u32 unk10_1: 1;
    u32 unk10_2: 1;
    u32 unk10_3 : 1;

    u32 unk14: 29;
    u32 unk14_1: 1;
    u32 unk14_2: 1;
    u32 unk14_3 : 1;

    struct WorkListEntry* pNext;
} WorkListEntry;


/*
// .sbss
s32 D_80059184;
int g_NumTimerWorkListEntries;
int g_NumWorkListEntries;
WorkListEntry* D_800594C0; // Last processed entry?
WorkListEntry* g_TimerWorkList;
WorkListEntry* D_80059590; // Next entry?
WorkListEntry* g_WorkList;

// other
extern u8 D_800591AF; // Heap alloc flag
extern int g_WorkListCurTimer; // Timer?
extern s32 D_80059464;
extern short D_80059494; // Set to 0 when timer above reaches 0

*/


INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/work_list", WorkListsFreeAllEntries);
/*
void WorkListFreeAllEntries(void) {
    WorkListEntry* pList;

    for (pList = g_TimerWorkList; g_TimerWorkList != NULL; pList = g_TimerWorkList) {
        pList->onFreeCallback(pList);
    }

    for (pList = g_WorkList; g_WorkList != NULL; pList = g_WorkList) {
        pList->onFreeCallback(pList);
    }
}
*/

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/work_list", WorkListsReset);
/*
void WorkListsReset(void) {
    g_TimerWorkList = NULL;
    g_WorkList = NULL;
    g_NumTimerWorkListEntries = 0;
    g_NumWorkListEntries = 0;
    g_WorkListCurTimer = 0;
}
*/

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/work_list", TimerWorkListUpdate);
/*
void TimerWorkListUpdate(void) {
    WorkListCallback_t pFnCallback;
    WorkListEntry* pEntry;

    // Count down timer
    if (g_WorkListCurTimer) {
        g_WorkListCurTimer--;
        if (g_WorkListCurTimer == 0) {
            D_80059494 = 0;
        }
    
    // Timer has reached zero
    } else {
        D_80059590 = g_TimerWorkList;
        while (D_80059590) {
            pEntry = D_80059590;
            D_800594C0 = pEntry;
            D_80059590 = pEntry->pNext;
            pFnCallback = pEntry->onTriggerCallback;
            if (pFnCallback) {
                pFnCallback(pEntry);
            }
        }  
    }
}
*/

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/work_list", WorkListUpdate);
/*
void WorkListUpdate(void) {
    WorkListCallback_t pFnCallback;
    WorkListEntry* pEntry;

    D_80059590 = g_WorkList;
    while (D_80059590) {
        pEntry = D_80059590;
        D_800594C0 = pEntry;
        D_80059590 = pEntry->pNext;
        pFnCallback = pEntry->onTriggerCallback; 
        if (pFnCallback) {
            pFnCallback(pEntry);
        }
    }
}
*/

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/work_list", WorkListAddTask);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/work_list", WorkListAllocateTask);
/*
WorkListEntry* WorkListAllocateTask(void* data, int dataSize) {
    WorkListEntry* pEntry;

    pEntry = HeapAlloc(dataSize + sizeof(WorkListEntry), D_800591AF);
    func_8001CA58(data, pEntry);
    pEntry->onFreeCallback = &TimerWorkListDeleteTask;
    return pEntry;
}
*/

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/work_list", WorkListRemoveTask);
/*
void WorkListRemoveTask(WorkListEntry* pTargetEntry) {
    WorkListEntry* pPrevEntry;
    WorkListEntry* pCurEntry;

    pPrevEntry = NULL;
    for (pCurEntry = g_WorkList; pCurEntry != NULL; pCurEntry = pCurEntry->pNext) {
        if (pCurEntry == pTargetEntry) {
            if (pPrevEntry) {
                pPrevEntry->pNext = pCurEntry->pNext;
            } else {
                g_WorkList = pCurEntry->pNext;
            }
            
            if (D_80059590 == pTargetEntry) {
                D_80059590 = pTargetEntry->pNext;
            }

            break;
        }

        pPrevEntry = pCurEntry;
    } 
    
    // Target entry was not found
    if (pCurEntry == NULL) {
        g_NumWorkListEntries++;
    }
    
    g_NumWorkListEntries--;
}
*/

void WorkListDeleteTask(WorkListEntry* pTask) {
    WorkListRemoveTask(pTask);
    HeapFree(pTask);
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/work_list", TimerWorkListAddTask);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/work_list", TimerWorkListAllocateTask);

void WorkListSetTaskCallback(WorkListEntry* pTask, WorkListCallback_t callback) {
    pTask->onTriggerCallback = callback;
}

void TimerWorkListSetTaskCallback(WorkListEntry* pTask, WorkListCallback_t callback) {
    pTask->onTriggerCallback = callback;
}

void WorkListTaskSetOnFreeCallback(WorkListEntry* pTask, WorkListCallback_t callback) {
    pTask->onFreeCallback = callback;
}

WorkListCallback_t WorkListTaskGetTaskCallback(WorkListEntry* pTask) {
    return pTask->onTriggerCallback;
}

WorkListCallback_t WorkListTaskGetOnFreeCallback(WorkListEntry* pTask) {
    return pTask->onFreeCallback;
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/work_list", TimerWorkListRemoveTask);
/*
void TimerWorkListRemoveTask(WorkListEntry* pTargetEntry) {
    WorkListEntry* pPrevEntry;
    WorkListEntry* pCurEntry;

    pPrevEntry = NULL;
    for (pCurEntry = g_TimerWorkList; pCurEntry != NULL; pCurEntry = pCurEntry->pNext) {
        if (pCurEntry == pTargetEntry) {
            if (pPrevEntry) {
                pPrevEntry->pNext = pCurEntry->pNext;
            } else {
                g_TimerWorkList = pCurEntry->pNext;
            }
            
            if (D_80059590 == pTargetEntry) {
                D_80059590 = pTargetEntry->pNext;
            }

            break;
        }

        pPrevEntry = pCurEntry;
    } 
    
    if (pTargetEntry->unk14_3) {
        D_80059464--;
    }
    
    g_NumTimerWorkListEntries--;
}
*/

void TimerWorkListDeleteTask(WorkListEntry* pTask) {
    TimerWorkListRemoveTask(pTask);
    HeapFree(pTask);
}

// Unlink target entry from lists if certain flags are met
INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/work_list", func_8001CE74);
/*
void func_8001CE74(WorkListEntry* pTargetEntry) {
    WorkListCallback_t pFnOnDeleteCallback;
    WorkListEntry* pCurEntry;
    WorkListEntry* pPrevEntry;

    pPrevEntry = NULL;
    for (pCurEntry = g_WorkList; pCurEntry != NULL; pCurEntry = pCurEntry->pNext) {
        if (
            pCurEntry->unk0 == pTargetEntry && 
            (pCurEntry->unk14_2 & 1) == 0 && 
            pCurEntry->unk14 == pTargetEntry->unk10
        ) {
            if (pPrevEntry) {
                pPrevEntry->pNext = pCurEntry->pNext;
            } else {
                g_WorkList = pCurEntry->pNext;
            }
            if (D_80059590 == pCurEntry) {
                D_80059590 = pCurEntry->pNext;
            }
            
            pFnOnDeleteCallback = pCurEntry->onFreeCallback;
            if (pFnOnDeleteCallback) {
                pFnOnDeleteCallback(pCurEntry);
            }
        } else {
            pPrevEntry = pCurEntry;
        }
    }

    pPrevEntry = NULL;
    for (pCurEntry = g_TimerWorkList; pCurEntry != NULL; pCurEntry = pCurEntry->pNext) {
        if (
            pCurEntry->unk0 == pTargetEntry && 
            (pCurEntry->unk14_2 & 1) == 0 && 
            (pCurEntry->unk14 & 0x1FFFFFFF) == pTargetEntry->unk10
        ) {
            if (pPrevEntry) {
                pPrevEntry->pNext = pCurEntry->pNext;
            } else {
                g_TimerWorkList = pCurEntry->pNext;
            }
            if (D_80059590 == pCurEntry) {
                D_80059590 = pCurEntry->pNext;
            }
            
            pFnOnDeleteCallback = pCurEntry->onFreeCallback;
            if (pFnOnDeleteCallback) {
                pFnOnDeleteCallback(pCurEntry);
            }
        } else {
            pPrevEntry = pCurEntry;
        }
    }
}
*/

// Set pCurEntry->unk4->unk70 of target entry if unk14_1 flag is set
INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/work_list", func_8001D034);
/*
void func_8001D034(WorkListEntry* pTargetEntry) {
    WorkListEntry* pCurEntry;

    for (pCurEntry = g_TimerWorkList; pCurEntry != NULL; pCurEntry = pCurEntry->pNext) {
        if (pCurEntry->unk0 == pTargetEntry) {
            if (
                pCurEntry->unk14 == pTargetEntry->unk10 &&
                (pCurEntry->unk14_1 & 1)
            ) {
                *(u32*)(pCurEntry->unk4 + 0x70) = 0;
            }
        }   
    }
}
*/

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/work_list", func_8001D0A4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/work_list", func_8001D10C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/work_list", func_8001D164);

void WorkListsDeleteTasks(WorkListEntry* pTasks) {
    WorkListRemoveTask(pTasks + 1);
    TimerWorkListRemoveTask(pTasks);
    HeapFree(pTasks);
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/work_list", WorkListsAddTasks);



// This may be the start of a new TU
INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/work_list", func_8001D298);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/work_list", func_8001D2A4);

// Change Sprite Animation Frame maybe?
INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/work_list", func_8001D2B0);

// Unlink SpriteData entry from list
INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/work_list", func_8001D3F4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/work_list", func_8001D468);

