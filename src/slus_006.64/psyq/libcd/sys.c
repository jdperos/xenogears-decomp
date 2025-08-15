#include "common.h"
#include "psyq/libcd.h"

extern char D_80018CE4; // "none"

/**
 * State
 */
extern unsigned char CD_status;
extern unsigned char CD_mode;
extern unsigned char CD_com;
extern CdlLOC CD_pos;
extern int CD_debug;

/**
 * Callbacks
 */
extern CdlCB CD_cbsync;
extern CdlCB CD_cbready;

extern char* CD_comstr[];
extern char* CD_intstr[];
extern int setloc[];

typedef void (*VoidCallback_t)(void);

int CdStatus(void) {
    return CD_status;
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

static int CD_control(u8 com, u8* param, u8* result, int arg3) {
    int i;
    void* prev_cb = CD_cbsync;

    for (i = 3; i != -1; i--) {
        CdSyncCallback(NULL);

        if ((com != 1) && (CdStatus() & CdlStatShellOpen)) {
            CD_cw(1, NULL, NULL, 0);
        }

        if (param == 0 || setloc[com] == 0 ||
            CD_cw(CdlSetloc, param, result, 0) == 0) {
            CdSyncCallback(prev_cb);
            if (CD_cw(com, param, result, arg3) == 0) {
                return 0;
            }
        }
    }

    CdSyncCallback(prev_cb);
    return -1;
}

int CdControl(u_char com, u_char* param, u_char* result) {
    return CD_control(com, param, result, 0) != -1;
}

int CdControlF(u_char com, u_char* param) {
    return CD_control(com, param, 0, 1) != -1;
}

int CdControlB(u_char com, u_char* param, u_char* result) {
    if (CD_control(com, param, result, 0)) {
        return 0;
    }

    return CD_sync(0, result) == 2;
}

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

void CdDataCallback(VoidCallback_t fn) {
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
