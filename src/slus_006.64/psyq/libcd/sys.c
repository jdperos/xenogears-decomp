#include "common.h"
#include "psyq/libcd.h"

extern char D_80018CE4; // "none"
extern char D_80018CCC;

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

extern int CD_comstr[];
extern char* CD_intstr[];

void _cbsync(void);
void _cbready(void);
void _cbread(void);

typedef void (*VoidCallback_t)(void);

int CdInit(void) {
    int i;
    int nStatus;
    int nResult;

    i = 4;
    while (i != -1) {
        nStatus = CdReset(1);
        i--;
        if (nStatus == 1) {
            CdSyncCallback(&_cbsync);
            CdReadyCallback(&_cbready);
            CdReadCallback(&_cbread);
            CdReadMode(0);
            nResult = 1;
            break;
        }
    
        if (i != -1) {
            continue;
        }
    
        printf(&D_80018CCC);
        nResult = 0;
        break;
    }
    
    return nResult;   
}


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

char* CdComstr(u_char com) {
    if (com >= 0x1c) {
        return &D_80018CE4;
    }
    return CD_comstr[com];
}

char* CdIntstr(u_char intr) {
    if (intr >= 7) {
        return &D_80018CE4;
    }
    return CD_intstr[intr];
}

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

void CdDataSyncCallback(VoidCallback_t fn) {
    DMACallback(3, fn);
}

void CdDataSync(int mode) { 
    CD_datasync(mode);
}

CdlLOC* CdIntToPos(int i, CdlLOC* p) {
    inline int ENCODE_BCD(n) { return ((n / 10) << 4) + (n % 10); }

    i += 150;
    p->sector = ENCODE_BCD(i % 75);
    p->second = ENCODE_BCD(i / 75 % 60);
    p->minute = ENCODE_BCD(i / 75 / 60);
    return p;
}

int CdPosToInt(CdlLOC* p) {
    u8 nSector;
    u8 nSecond;
    u8 nMinute;
    nMinute = p->minute;
    nSecond = p->second;
    nSector = p->sector;
    return (((((((nMinute >> 4) * 10) + (nMinute & 0xF)) * 60) + 
              (((nSecond >> 4) * 10) + (nSecond & 0xF))) * 75) + 
              (((nSector >> 4) * 10) + (nSector & 0xF))) - 150;
}
