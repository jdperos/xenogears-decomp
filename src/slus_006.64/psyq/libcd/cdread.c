#include "common.h"
#include "psyq/libcd.h"
#include "psyq/stdio.h"

#define NONBLOCKING 0x1
#define BLOCKING 0x0
#define READ_TIMEOUT_VBLANKS 0x4B0
#define ATTEMPT_TIMEOUT_VBLANKS 0x3C

/**
 * Internal types
 * TODO: Finish naming unknowns.
 */
typedef struct {
    int nsectors;      // Total number of sectors to read in this request.
    u_long* dest_base; // Pointer to start of destination buffer.
    u_long* dest_cur;  // Pointer to next write position in destination buffer.
    int mode;          // CD-ROM read mode (e.g., CdlMode).
    int payload_words; // Words of user data per sector.
    int status;        // Remaining sectors to read, or -1 if an error occurred.
    int vb_attempt;    // VBlank count at start of current read attempt.
    int vb_read;       // VBlank count at the start of the first read attempt.
    int sector_num;    // Logical sector number currently being read.
    CdlCB cbsync;
    CdlCB cbready;
    CdlCB cbdata;
    int block_mode; // Read mode: 0 = blocking, 1 = non-blocking.
    u_char* unk34;
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

/**
 * Strings
 * TODO: Figure out what these are.
 */
extern char* D_80018F44;
extern char* D_80018F5C;
extern char* D_80018F74;

extern int Vsync(int mode);

void cb_read(u_char, u_char*);
void cb_data(void);

void cb_read(u_char status, u_char res[2]) {
    u_long tmp_buf[3];
    ReadAttr.unk34 = res;
    if (status == CdlDataReady) {
        if (ReadAttr.status > 0) {
            // For 2048-byte user-data sectors (Mode 2 Form 1), read last 12
            // bytes of the sector header (address, mode, subheader).
            if (ReadAttr.payload_words == 2048 / 4) {
                if (ReadAttr.block_mode & NONBLOCKING) {
                    // Temporarily disable data callback so the header read
                    // doesn’t trigger the normal data processing.
                    CdDataSyncCallback(NULL);
                    CdGetSector2(&tmp_buf, /*size=*/3);
                    CdDataSync(BLOCKING);
                    CdDataSyncCallback(&cb_data); // Restore data callback.
                } else {
                    CdGetSector(&tmp_buf, /*size=*/3);
                }
                // Compare address to expected position.
                if (CdPosToInt(&tmp_buf) != ReadAttr.sector_num) {
                    puts((char*)&D_80018F44);
                    ReadAttr.status = -1;
                }
            }
            // Read full sector payload into destination buffer.
            if (ReadAttr.block_mode & NONBLOCKING) {
                CdGetSector2(ReadAttr.dest_cur, ReadAttr.payload_words);
            } else {
                CdGetSector(ReadAttr.dest_cur, ReadAttr.payload_words);
                ReadAttr.dest_cur += ReadAttr.payload_words;
                --ReadAttr.status;
                ++ReadAttr.sector_num;
            }
        }
    } else {
        ReadAttr.status = -1;
    }
    ReadAttr.vb_attempt = Vsync(-1);

    if (ReadAttr.status < 0) {
        cd_read_retry(/*is_retry=*/true);
    }

    if (Vsync(-1) > ReadAttr.vb_read + READ_TIMEOUT_VBLANKS) {
        ReadAttr.status = -1;
    }

    if (ReadAttr.status != 0 &&
        Vsync(-1) <= ReadAttr.vb_read + READ_TIMEOUT_VBLANKS) {
        return;
    }

    CdSyncCallback(ReadAttr.cbsync);
    CdReadyCallback(ReadAttr.cbready);

    if (ReadAttr.block_mode & NONBLOCKING) {
        CdDataSyncCallback(ReadAttr.cbdata);
    }

    CdControlF(CdlPause, 0);

    if (CD_cbread == NULL) {
        return;
    }

    CD_cbread(ReadAttr.status == 0 ? 2 : 5, res);
}

void cb_data(void) {
    ReadAttr.dest_cur += ReadAttr.payload_words;
    --ReadAttr.status;
    ++ReadAttr.sector_num;

    if (ReadAttr.status != 0) {
        return;
    }

    CdSyncCallback(ReadAttr.cbsync);
    CdReadyCallback(ReadAttr.cbready);

    if (ReadAttr.block_mode & NONBLOCKING) {
        CdDataSyncCallback(ReadAttr.cbdata);
    }

    CdControlF(CdlPause, 0);

    if (CD_cbread != NULL) {
        CD_cbread(2, ReadAttr.unk34);
    }
}

int cd_read_retry(bool is_retry) {
    u8 mode;
    // Disable any existing callbacks for sync, ready and data.
    CdSyncCallback(NULL);
    CdReadyCallback(NULL);
    if (ReadAttr.block_mode & NONBLOCKING) {
        CdDataSyncCallback(NULL);
    }

    // Handle case where the drive tray is open.
    if (CdStatus() & CdlStatShellOpen) {
        if ((Vsync(-1) & 0x3F) == 0) {
            puts((char*)&D_80018F5C);
        }
        CdControlF(CdlNop, 0);
        ReadAttr.vb_read = Vsync(-1);
        ReadAttr.status = -1;
        return ReadAttr.status;
    }

    if (is_retry) {
        puts((char*)&D_80018F74);
        CdControl(CdlPause, 0, 0);
        if (!CdControl(CdlSetloc, CdLastPos(), 0)) {
            ReadAttr.status = -1;
            return ReadAttr.status;
        }
    }

    CdFlush();

    mode = ReadAttr.mode;
    // Set mode if changed or retrying.
    if (mode != CdMode() || is_retry) {
        if (!CdControl(CdlSetmode, &mode, 0)) {
            ReadAttr.status = -1;
            return ReadAttr.status;
        }
    }

    // Save current sector number.
    ReadAttr.sector_num = CdPosToInt(CdLastPos());

    // Install read callback.
    CdReadyCallback(&cb_read);

    // Install data sync callback if Non-blocking mode.
    if (ReadAttr.block_mode & NONBLOCKING) {
        CdDataSyncCallback(&cb_data);
    }

    ReadAttr.dest_cur = ReadAttr.dest_base;

    // Issue read with retry command.
    CdControlF(CdlReadN, 0);

    ReadAttr.status = ReadAttr.nsectors;
    ReadAttr.vb_attempt = Vsync(-1);

    return ReadAttr.status;
}

void CdReadBreak(void) {
    if (ReadAttr.block_mode & NONBLOCKING) {
        CdDataSync(0);
    }

    ReadAttr.status = 0;
    CdSyncCallback(ReadAttr.cbsync);
    CdReadyCallback(ReadAttr.cbready);

    if (ReadAttr.block_mode & NONBLOCKING) {
        CdDataSyncCallback(ReadAttr.cbdata);
    }

    CdControlF(CdlPause, 0);
}

int CdRead(int sectors, u_long* buf, int mode) {
    ReadAttr.mode = mode;
    switch (ReadAttr.mode & (CdlModeSize0 | CdlModeSize1)) {
    case 0:
        ReadAttr.payload_words = 2048 / sizeof(u_long);
        break;
    case CdlModeSize1:
        ReadAttr.payload_words = 2340 / sizeof(u_long);
        break;
    default: // CdlModeSize0 set
        ReadAttr.payload_words = 2328 / sizeof(u_long);
    }
    ReadAttr.mode |= 0x20;
    ReadAttr.dest_base = buf;
    ReadAttr.nsectors = sectors;
    ReadAttr.cbsync = CdSyncCallback(NULL);
    ReadAttr.cbready = CdReadyCallback(NULL);
    if (ReadAttr.block_mode & NONBLOCKING) {
        ReadAttr.cbdata = CdDataSyncCallback(NULL);
    }
    ReadAttr.vb_read = Vsync(-1);
    if (CdStatus() & (CdlStatPlay | CdlStatSeek | CdlStatRead)) {
        CdControlB(CdlPause, 0, 0);
    }
    return cd_read_retry(/*is_retry=*/false) > 0;
}

int CdReadSync(int mode, u_char* result) {
    int status;
    while (true) {
        status = -1;
        if (Vsync(-1) <= ReadAttr.vb_read + READ_TIMEOUT_VBLANKS) {
            if (ReadAttr.status < 0 ||
                Vsync(-1) > ReadAttr.vb_attempt + ATTEMPT_TIMEOUT_VBLANKS) {
                cd_read_retry(/*is_retry=*/true);
                status = ReadAttr.nsectors;
            } else {
                status = ReadAttr.status;
            }
        }
        if (mode != BLOCKING || status <= 0) {
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
    prev = ReadAttr.block_mode;
    ReadAttr.block_mode = mode;
    return prev;
}
