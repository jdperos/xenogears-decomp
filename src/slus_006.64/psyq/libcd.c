#include "common.h"
#include "psyq/libdma.h"
#include "psyq/libcd.h"
#include "psyq/stdio.h"
/**
  Internal types
*/
typedef struct {
    unsigned char sync;
    unsigned char ready;
    unsigned char c;
} Intr_t;

typedef struct Result_t {
    int unk0;
    int unk4;
} Result_t;

typedef struct Alarm_t {
    int unk0;
    int unk4;
    char* unk8;
} Alarm_t;

extern int CD_debug;
extern CdCallbackFn_t* CD_cbsync;
extern CdCallbackFn_t* CD_cbready;
extern void callback;

/**
  State.
 */
extern unsigned char CD_mode;
extern unsigned char CD_com;
extern int CD_status;
extern int CD_status1;
extern CdlLOC CD_pos;
extern volatile Intr_t Intr;
extern volatile Alarm_t Alarm;
extern int ComAttr[];
extern Result_t Result[3];

/**
  CD Drive + DMA Registers.
*/
extern volatile u8* reg0; // CD Index/Status.
extern volatile u8* reg1;
extern volatile u8* reg2;
extern volatile u8* reg3; // CD Interrupt Enable.
extern volatile u32* com_delay;
extern volatile u32* cdrom_delay;
extern volatile u32* d_pcr;
extern volatile u32* d3_madr;
extern volatile u32* d3_bcr;
extern volatile u32* d3_chcr;

extern u32 D_8005A234;
extern u8 D_800567C1[];
extern char D_80018F18;
extern char D_80018F24;
extern int D_8005678C;
extern volatile u32* _spu;

/**
  Strings
 */
extern int CD_comstr[];
//  char* CD_comstr[32] = {"CdlSync",    "CdlNop",
//      "CdlSync",    "CdlNop",
//      "CdlSetloc",  "CdlPlay",
//      "CdlForward", "CdlBackward",
//      "CdlReadN",   "CdlStandby",
//      "CdlStop",    "CdlPause",
//      "CdlReset",   "CdlMute",
//      "CdlDemute",  "CdlSetfilter",
//      "CdlSetmode", "?",
//      "CdlGetlocL", "CdlGetlocP",
//      "?",          "CdlGetTN",
//      "CdlGetTD",   "CdlSeekL",
//      "CdlSeekP",   "?",
//      "?",          "?",
//      "?",          "CdlReadS",
//      "?",          "?",
//      "?",          "?",
//  };

extern char* CD_intstr[];
//  char* CD_intstr[8] = {
//      "NoIntr",  "DataReady", "Complete", "Acknowledge",
//      "DataEnd", "DiskError", "?",        "?",
//  };

extern char* CD_str0; // "CD timeout: "
extern char* CD_str1; // "%s:(%s) Sync=%s, Ready=%s\n"
extern char* CD_str2; // "%s...\n"
extern char* CD_str3; // "%s: no param\n"
extern char* CD_str4; // "CD_cw"

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CdInit);

void _cbsync(void) {
    DeliverEvent(0xF0000003, 0x20);
}

void _cbready(void) {
    DeliverEvent(0xF0000003, 0x40);
}

void _cbread() {
    DeliverEvent(0xF0000003, 0x40);
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", DeliverEvent);

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
    prevCallback = CD_cbsync;
    CD_cbsync = callback;
    return prevCallback;
}

CdCallbackFn_t* CdReadyCallback(CdCallbackFn_t* callback) {
    CdCallbackFn_t* prevCallback;
    prevCallback = CD_cbready;
    CD_cbready = callback;
    return prevCallback;
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CdControl);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CdControlF);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CdControlB);

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

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CdDataSyncCallback);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CdDataSync);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CdIntToPos);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CdPosToInt);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", getintr);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CD_sync);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CD_ready);

extern int Vsync(int mode);
extern int CheckCallback();
extern int getintr();

static inline void handle_cb() {
    u8 masked;
    u32 status;

    masked = *reg0 & 0x3;
    while ((status = getintr())) {
        if ((status & 0x4) != 0 && CD_cbready) {
            CD_cbready(Intr.ready, (void*)&Result[1]);
        }
        if ((status & 0x2) != 0 && CD_cbsync) {
            CD_cbsync(Intr.sync, (void*)&Result);
        }
    }
    *reg0 = masked;
}

static inline void _memcpy(void* _dst, void* _src, u32 _size) {
    char* pDst = (char*)_dst;
    char* pSrc = (char*)_src;

    if (pDst == 0)
        return;

    while (_size--) {
        *pDst++ = *pSrc++;
    }
}

int CD_cw(u8 arg0, u8* arg1, u8* arg2, int arg3) {
    int i;
    int res;

    if (!(CD_debug < 2)) {
        printf((char*)&CD_str2, CD_comstr[arg0]);
    }

    if ((ComAttr[0x40 + arg0] != 0) && (arg1 == 0)) {
        if (CD_debug > 0) {
            printf((char*)&CD_str3, CD_comstr[arg0]);
        }
        return -2;
    }

    CD_sync(0, 0);

    if (arg0 == 0x2) {
        for (i = 0; i < 0x4; ++i) {
            *(u8*)((u32)&CD_pos + i) = arg1[i];
        }
    }

    if (arg0 == 0xE) {
        CD_mode = *arg1;
    }

    Intr.sync = 0;
    if (ComAttr[arg0] != 0) {
        Intr.ready = 0;
    }

    *reg0 = 0;
    for (i = 0; i < ComAttr[0x40 + arg0]; ++i) {
        *reg2 = arg1[i];
    }
    CD_com = arg0;
    *reg1 = CD_com;

    if (arg3 != 0)
        return 0;

    ((Alarm_t*)&Alarm)->unk0 = Vsync(-1) + 0x3C0;
    ((Alarm_t*)&Alarm)->unk4 = 0;
    ((Alarm_t*)&Alarm)->unk8 = (char*)&CD_str4;

    while (Intr.sync == 0) {
        if (((Alarm_t*)&Alarm)->unk0 < Vsync(-1) ||
            ((Alarm_t*)&Alarm)->unk4++ > 0x3C0000) {
            puts((char*)&CD_str0);
            printf((char*)&CD_str1, ((Alarm_t*)&Alarm)->unk8, CD_comstr[CD_com],
                   CD_intstr[Intr.sync], CD_intstr[Intr.ready]);
            CD_flush();
            res = -1;
        } else {
            res = 0;
        }

        if (res != 0) {
            return -1;
        }

        if (CheckCallback() != 0) {
            handle_cb();
        }
    }

    _memcpy(arg2, &Result, sizeof(Result[0]));
    return -!(Intr.sync ^ 0x5);
}

int CD_vol(CdlATV* vol) {
    *reg0 = 2;
    *reg2 = vol->val0;
    *reg3 = vol->val1;
    *reg0 = 3;
    *reg1 = vol->val2;
    *reg2 = vol->val3;
    *reg3 = 0x20;
    return 0;
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CD_flush);

int CD_initvol(void) {
    CdlATV vol;
    // 0x1F801c00 + 0x1b8 = 0x1F801db8 (current main vol left).
    if (*((volatile u16*)((u32)_spu + 0x1b8)) == 0 &&
        *((volatile u16*)((u32)_spu + 0x1ba)) == 0) {
        // 0x1F801c00 + 0x180 = 0x1F801d80 (main vol left/right).
        *((volatile u16*)((u32)_spu + 0x180)) = 0x3fff;
        *((volatile u16*)((u32)_spu + 0x182)) = 0x3fff;
    }
    // 0x1F801c00 + 0x1b0 = 0x1F801db0 (CD volume left/right).
    *((volatile u16*)((u32)_spu + 0x1b0)) = 0x3fff;
    *((volatile u16*)((u32)_spu + 0x1b2)) = 0x3fff;
    // Enable spu, unmute spu and enable cd audio.
    *((volatile u16*)((u32)_spu + 0x1aa)) = (1 << 15) | (1 << 14) | (1 << 0);
    vol.val0 = vol.val2 = 0x80;
    vol.val1 = vol.val3 = 0;
    *reg0 = 2;
    *reg2 = vol.val0;
    *reg3 = vol.val1;
    *reg0 = 3;
    *reg1 = vol.val2;
    *reg2 = vol.val3;
    *reg3 = 0x20;
    return 0;
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CD_initintr);

int CD_init(void) {
    puts(&D_80018F18);
    printf(&D_80018F24, &D_8005678C);
    CD_com = 0;
    CD_mode = 0;
    CD_cbready = 0;
    CD_cbsync = 0;
    CD_status1 = 0;
    CD_status = 0;
    ResetCallback();
    InterruptCallback(2, &callback);

    *reg0 = 1;
    while (*reg3 & 7) {
        *reg0 = 1;
        *reg3 = 7;
        *reg2 = 7;
    }

    Intr.ready = Intr.c = 0;
    Intr.sync = 2;

    *reg0 = 0;
    *reg3 = 0;
    *com_delay = 0x1325;

    CD_cw(1, 0, 0, 0);
    if (CD_status & 0x10) {
        CD_cw(1, 0, 0, 0);
    }

    if (CD_cw(0xA, 0, 0, 0) != 0) {
        return -1;
    }

    if (CD_cw(0xC, 0, 0, 0) != 0) {
        return -1;
    }

    if (CD_sync(0, 0) != 2) {
        return -1;
    }

    return 0;
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CD_datasync);

int CD_getsector(void* madr, size_t size) {
    *reg0 = 0;
    *reg3 = 0x80;
    *cdrom_delay = 0x20943;
    *com_delay = 0x1323;
    // DMA3 Master Enable.
    *d_pcr |= 0x8000;
    *d3_madr = (u32)madr;
    *d3_bcr = size | 0x10000;
    while (!(*reg0 & 0x40))
        ;
    // Start DMA transfer.
    *d3_chcr = DMA_CHCR_CDROM_NORMAL;
    // Wait until transfer is complete.
    while (*d3_chcr & DMA_CHCR_START_BUSY_ENABLED)
        ;
    *com_delay = 0x1325;
    return 0;
}

int CD_getsector2(void* madr, size_t size) {
    volatile u32 sp0;
    *reg0 = 0;
    *reg3 = 0x80;
    *cdrom_delay = 0x21020843;
    *com_delay = 0x1325;
    *d_pcr |= 0x8000;
    *d3_madr = (u32)madr;
    *d3_bcr = size | 0x10000;
    while (!(*reg0 & 0x40))
        ;
    // Start DMA transfer w/ "chopping" (cycle stealing by CPU).
    *d3_chcr = DMA_CHCR_CDROM_CHOPPED;
    sp0 = *d3_chcr;
    return 0;
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_80042C98);
INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", callback);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", puts);

void func_80042DDC(u8 byte) {
    if (byte != 0x9) {
        if (byte == 0xa) {
            func_80042DDC(0xd);
            D_8005A234 = 0;
            write(1, &byte, 1);
            return;
        }
    } else {
        do {
            func_80042DDC(0x20);
        } while (D_8005A234 & 0x7);
        return;
    }

    if (D_800567C1[byte] & 0x97) {
        D_8005A234++;
    }

    write(1, &byte, 1);
}

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