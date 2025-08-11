#include "common.h"
#include "psyq/libcd.h"

/**
 * Internal types
 */
typedef struct {
    int nsectors;
    u_long* buf;
    int unk8;
    int mode;
    int secsize;
    int status;
    int unk18;
    int vb_start;
    int unk20;
    CdlCB prev_cbsync;
    CdlCB prev_cbready;
    CdlCB prev_cbdata;
    int nonblock; // 0: Blocking, 1: Non-blocking
    int unk34;
    int unk38;
} ReadAttr_t;

/**
 * Callbacks
 */
extern CdlCB CD_cbread;

/**
 * State
 */
extern volatile ReadAttr_t ReadAttr;

extern int Vsync(int mode);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd/cdread", cb_read);
INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd/cdread", cb_data);
INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd/cdread", cd_read_retry);
INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd/cdread", CdReadBreak);

int CdRead(int sectors, u_long* buf, int mode) {
    ReadAttr.mode = mode;
    switch (ReadAttr.mode & (CdlModeSize0 | CdlModeSize1)) {
    case (0):
        ReadAttr.secsize = 2048 / 4;
        break;
    case (CdlModeSize1):
        ReadAttr.secsize = 2340 / 4;
        break;
    default: /* CdlModeSize0 set (alone or with CdlModeSize1) */
        ReadAttr.secsize = 2328 / 4;
    }
    ReadAttr.mode |= 0x20;
    ReadAttr.buf = buf;
    ReadAttr.nsectors = sectors;
    ReadAttr.prev_cbsync = CdSyncCallback(0);
    ReadAttr.prev_cbready = CdReadyCallback(0);
    if (ReadAttr.nonblock & 0x1) {
        ReadAttr.prev_cbdata = CdDataSyncCallback(0);
    }
    ReadAttr.vb_start = Vsync(-1);
    if (CdStatus() & (CdlStatPlay | CdlStatSeek | CdlStatRead)) {
        CdControlB(CdlPause, 0, 0);
    }
    return cd_read_retry(0) > 0;
}

int CdReadSync(int mode, u_char* result) {
    int status;
    while (true) {
        status = -1;
        if (Vsync(-1) <= ReadAttr.vb_start + 0x4B0) {
            if (ReadAttr.status < 0 || Vsync(-1) > ReadAttr.unk18 + 0x3C) {
                cd_read_retry(1);
                status = ReadAttr.nsectors;
            } else {
                status = ReadAttr.status;
            }
        }
        if (mode != 0 || status <= 0) {
            CdReady(1, result);
            return status;
        }
    }
}

CdlCB CdReadCallback(CdlCB func) {
    CdlCB prev;
    prev = CD_cbread;
    CD_cbread = func;
    return prev;
}

int CdReadMode(int mode) {
    int prev;
    prev = ReadAttr.nonblock;
    ReadAttr.nonblock = mode;
    return prev;
}
