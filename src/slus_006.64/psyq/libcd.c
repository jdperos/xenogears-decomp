#include "common.h"
#include "psyq/libcd.h"

extern int g_CdDebugLevel;
extern CdCallbackFn_t* g_CdSyncCallback;
extern CdCallbackFn_t* g_CdReadyCallback;

extern unsigned char g_CdMode;
extern unsigned char g_CdLastCommand;
extern unsigned char g_CdStatus;
extern CdlLOC g_CdLastPosition;

extern volatile u8* g_CdIndexStatusReg;
extern volatile u8* g_CdInterruptEnableReg;
extern volatile u32* g_ComDelayReg;
extern volatile u32* g_CdRomDelayReg;
extern volatile u32* g_DmaControlReg;
extern volatile u32* g_Dma3AddrReg;
extern volatile u32* g_Dma3BlockControlReg;
extern volatile u32* g_Dma3ChanControlReg;

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CdInit);

void _cbsync(void) { DeliverEvent(0xF0000003, 0x20); }

void _cbready(void) { DeliverEvent(0xF0000003, 0x40); }

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", _cbread);

// TODO: I really don't think this is supposed to be in libcd
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

CdlLOC* CdLastPos(void) { return &g_CdLastPosition; }

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

u32 CD_getsector(u32 madr, u32 size) {
    // Reset CD command register.
    *g_CdIndexStatusReg = 0;
    *g_CdInterruptEnableReg = 0x80; // Exact effect is unclear.

    // Configure CD-ROM read timing parameters
    *g_CdRomDelayReg = 0x20943;
    *g_ComDelayReg = 0x1323;

    // DMA3 Master Enable.
    *g_DmaControlReg |= 0x8000;

    // Address in RAM where CD data should be written.
    *g_Dma3AddrReg = madr;

    // Set DMA transfer size (words in one block).
    *g_Dma3BlockControlReg = size | 0x10000;

    while (!(*g_CdIndexStatusReg & 0x40 /*Data fifo empty*/))
        ;

    // Start DMA transfer using SyncMode 0, enable + trigger.
    *g_Dma3ChanControlReg = 0x11000000;

    // Wait until DMA3 is no longer busy
    while (*g_Dma3ChanControlReg & 0x01000000)
        ;

    *g_ComDelayReg = 0x1325;

    return 0;
}

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