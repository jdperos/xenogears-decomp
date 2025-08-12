#include "common.h"
#include "psyq/libcd.h"

/**
 * State
 */
extern int CD_status;
extern unsigned char CD_mode;
extern unsigned char CD_com;
extern CdlLOC CD_pos;
extern int CD_debug;

/**
 * Callbacks
 */
extern CdlCB CD_cbsync;
extern CdlCB CD_cbready;

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd/sys", CdInit);

void _cbsync(void) {
    DeliverEvent(0xF0000003, 0x20);
}

void _cbready(void) {
    DeliverEvent(0xF0000003, 0x40);
}

void _cbread() {
    DeliverEvent(0xF0000003, 0x40);
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd/sys", DeliverEvent);

int CdStatus(void) {
    // FIXME: CD_status gets loaded as a byte here for some reason.
    return (*((u8*)&CD_status));
}

int CdMode(void) {
    return CD_mode;
}

int CdLastCom(void) {
    return CD_com;
}

CdlLOC* CdLastPos(void) {
    return &CD_pos;
}

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

    prevLevel = CD_debug;
    CD_debug = level;
    return prevLevel;
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd/sys", CdComstr);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd/sys", CdIntstr);

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

CdlCB CdSyncCallback(CdlCB func) {
    CdlCB prevCallback;
    prevCallback = CD_cbsync;
    CD_cbsync = func;
    return prevCallback;
}

CdlCB CdReadyCallback(CdlCB func) {
    CdlCB prevCallback;
    prevCallback = CD_cbready;
    CD_cbready = func;
    return prevCallback;
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd/sys", CdControl);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd/sys", CdControlF);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd/sys", CdControlB);

int CdMix(CdlATV* vol) {
    CD_vol(vol);
    return 1;
}

int CdGetSector(void* madr, int size) {
    return CD_getsector(madr, size) == 0;
}

int CdGetSector2(void* madr, int size) {
    return CD_getsector2(madr, size) == 0;
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd/sys", CdDataSyncCallback);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd/sys", CdDataSync);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd/sys", CdIntToPos);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd/sys", CdPosToInt);
