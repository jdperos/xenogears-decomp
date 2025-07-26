#include "common.h"
#include "psyq/libcd.h"

extern int g_CdDebugLevel;
extern CdCallbackFn_t* g_CdSyncCallback;
extern CdCallbackFn_t* g_CdReadyCallback;

extern unsigned char g_CdMode;
extern unsigned char g_CdLastCommand;
extern unsigned char g_CdStatus;

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CdInit);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", _cbsync);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", _cbready);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", _cbread);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", DeliverEvent);

int CdStatus(void) {
    return g_CdStatus;
}

int CdMode(void) {
    return g_CdMode;
}

int CdLastCom(void) {
    return g_CdLastCommand;
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CdLastPos);

int CdReset(int mode) {
    if (mode == 2) {
        CD_initintr();
        return 1;
    }

    if (CD_init() != 0)
        return 0;

    if (mode == 1) {
        if (CD_initvol() != 0)
            return 0;
    }

    return 1;
}

void CdFlush(void) { 
    CD_flush(); 
}

int CdSetDebug(int level) {
    int prevLevel;
    
    prevLevel = g_CdDebugLevel;
    g_CdDebugLevel = level;
    return prevLevel;
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CdComstr);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CdIntstr);

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

int CdMix(CdlATV* vol) {
    CD_vol(vol);
    return 1;
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CdGetSector);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CdGetSector2);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CdDataSyncCallback);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CdDataSync);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CdIntToPos);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CdPosToInt);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CdReadInterrupt);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CD_sync);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CD_ready);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CD_cw);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CD_vol);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CD_flush);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CD_initvol);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CD_initintr);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CD_init);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CD_datasync);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CD_getsector);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CD_getsector2);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_80042C98);
INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_80042CA8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", puts);
INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_80042DDC);
INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_80042E90);
INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_80042EC0);
INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_80042EF0);
INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_800431C0);
INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_800432BC);
INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_800434D0);
INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_8004356C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CdReadSync);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CdReadCallback);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_80043754);