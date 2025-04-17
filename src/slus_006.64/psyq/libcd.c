#include "common.h"
#include "psyq/libcd.h"

extern int g_CdDebugLevel;
extern CdCallbackFn_t* g_CdSyncCallback;
extern CdCallbackFn_t* g_CdReadyCallback;
extern func_80042700(); // psyq/interrupts

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CdInit);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_80040DA0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_80040DC8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_80040DF0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_80040E18);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_80040E38);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_80040E48);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_80040E58);

//INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CdReset);
int CdReset(int mode) {
    if (mode == 2) {
        func_80042700();
        return 1;
    }

    if (CD_init() !=  0)
        return 0;

    if (mode == 1) {
        if (CD_initvol() != 0)
            return 0;
    }

    return 1;
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_80040ED4);

int CdSetDebug(int level) {
    int prevLevel;
    
    prevLevel = g_CdDebugLevel;
    g_CdDebugLevel = level;
    return prevLevel;
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_80040F0C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_80040F40);

int CdSync(int mode, u_char* result) {
    int nStatus;

    nStatus = CD_sync(mode, result);
    return nStatus;
}

int CdReady(int mode, u_char* result) {
    int nStatus;

    nStatus = CD_ready(mode, result);
    return nStatus;
}

CdCallbackFn_t* CdSyncCallback(CdCallbackFn_t* callback) {
    CdCallbackFn_t* prevCallback;
    prevCallback = g_CdSyncCallback;
    g_CdSyncCallback = callback;
    return prevCallback;
}

CdCallbackFn_t* CdReadyCallback(CdCallbackFn_t* callback) {
    CdCallbackFn_t* prevCallback;
    prevCallback = g_CdReadyCallback;
    g_CdReadyCallback = callback;
    return prevCallback;
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CdControl);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CdControlF);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CdControlB);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_8004138C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_800413AC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_800413CC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_800413EC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CdDataSync);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_80041430);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_80041534);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_800415B4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CD_sync);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CD_ready);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CD_cw);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_800424A4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CD_flush);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CD_initvol);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_80042700);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CD_init);
