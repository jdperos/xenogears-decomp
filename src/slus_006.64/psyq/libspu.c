#include "common.h"
#include "psyq/libspu.h"
#include "psyq/libapi.h"

typedef struct {
    SpuVolume volume;
    u16 pitch;
    u16 addr;
    u16 adsr[2];
    u16 volumex;
    u16 loop_addr;
} SPU_VOICE_REG;

typedef struct {
    // APF Displacement registers (1F801DC0h - 1F801DC2h)
    u16 m_dAPF1;
    u16 m_dAPF2;

    // Volume registers (1F801DC4h - 1F801DD2h)
    s16 m_vIIR;
    s16 m_vCOMB1;
    s16 m_vCOMB2;
    s16 m_vCOMB3;
    s16 m_vCOMB4;
    s16 m_vWALL;
    s16 m_vAPF1;
    s16 m_vAPF2;

    // Same Side Reflection Address registers (1F801DD4h - 1F801DD6h)
    u16 m_mLSAME;
    u16 m_mRSAME;

    // Comb Address registers (1F801DD8h - 1F801DDEh)
    u16 m_mLCOMB1;
    u16 m_mRCOMB1;
    u16 m_mLCOMB2;
    u16 m_mRCOMB2;

    // Same Side Reflection Address 2 registers (1F801DE0h - 1F801DE2h)
    u16 m_dLSAME;
    u16 m_dRSAME;

    // Different Side Reflection Address registers (1F801DE4h - 1F801DE6h)
    u16 m_mLDIFF;
    u16 m_mRDIFF;

    // Comb Address registers 3-4 (1F801DE8h - 1F801DEEh)
    u16 m_mLCOMB3;
    u16 m_mRCOMB3;
    u16 m_mLCOMB4;
    u16 m_mRCOMB4;

    // Different Side Reflection Address 2 registers (1F801DF0h - 1F801DF2h)
    u16 m_dLDIFF;
    u16 m_dRDIFF;

    // APF Address registers (1F801DF4h - 1F801DFAh)
    u16 m_mLAPF1;
    u16 m_mRAPF1;
    u16 m_mLAPF2;
    u16 m_mRAPF2;

    // Input Volume registers (1F801DFCh - 1F801DFEh)
    s16 m_vLIN;
    s16 m_vRIN;
} ReverbRegisters;

typedef struct {
    SPU_VOICE_REG voice[24];
    // Volumes
    SpuVolume main_vol; // 1-bit for Volume Mode, 15-bits for Volume
    SpuVolume rev_vol; // Full 16 bits for volume

    // Voice Flags
    u32 m_KeyOnFlags;
    u32 m_KeyOffFlags;
    u32 m_PitchModFlags;
    u32 m_NoiseFlags;
    u32 m_ReverbFlags;
    u32 m_EndxFlags;

    u16 m_Unknown;

    // Memory
    u16 m_ReverbWorkStartAddr;
    u16 m_IrqAddress;
    u16 trans_addr;
    u16 m_TransferFifo;

    // Control
    u16 spucnt;
    u16 m_TransferControl;
    u16 m_StatusRegister;

    // Aux volumes
    s16 m_CdInputVolumeL;
    s16 m_CdInputVolumeR;
    s16 m_ExtInputVolumeL;
    s16 m_ExtInputVolumeR;

    s16 m_CurrentMainVolL;
    s16 m_CurrentMainVolR;

    u32 m_Unknown2;

    ReverbRegisters m_Reverb;
} SPU_RXX;

union SpuUnion {
    SPU_RXX _rxx;
    volatile SPU_RXX rxx;
    u16 _raw[0x100];
    volatile u16 raw[0x100];
};

#define SPU_REV_MASK_dAPF1       (1U <<  0)
#define SPU_REV_MASK_dAPF2       (1U <<  1)
#define SPU_REV_MASK_vIIR        (1U <<  2)
#define SPU_REV_MASK_vCOMB1      (1U <<  3)
#define SPU_REV_MASK_vCOMB2      (1U <<  4)
#define SPU_REV_MASK_vCOMB3      (1U <<  5)
#define SPU_REV_MASK_vCOMB4      (1U <<  6)
#define SPU_REV_MASK_vWALL       (1U <<  7)
#define SPU_REV_MASK_vAPF1       (1U <<  8)
#define SPU_REV_MASK_vAPF2       (1U <<  9)
#define SPU_REV_MASK_mLSAME      (1U << 10)
#define SPU_REV_MASK_mRSAME      (1U << 11)
#define SPU_REV_MASK_mLCOMB1     (1U << 12)
#define SPU_REV_MASK_mRCOMB1     (1U << 13)
#define SPU_REV_MASK_mLCOMB2     (1U << 14)
#define SPU_REV_MASK_mRCOMB2     (1U << 15)
#define SPU_REV_MASK_dLSAME      (1U << 16)
#define SPU_REV_MASK_dRSAME      (1U << 17)
#define SPU_REV_MASK_mLDIFF      (1U << 18)
#define SPU_REV_MASK_mRDIFF      (1U << 19)
#define SPU_REV_MASK_mLCOMB3     (1U << 20)
#define SPU_REV_MASK_mRCOMB3     (1U << 21)
#define SPU_REV_MASK_mLCOMB4     (1U << 22)
#define SPU_REV_MASK_mRCOMB4     (1U << 23)
#define SPU_REV_MASK_dLDIFF      (1U << 24)
#define SPU_REV_MASK_dRDIFF      (1U << 25)
#define SPU_REV_MASK_mLAPF1      (1U << 26)
#define SPU_REV_MASK_mRAPF1      (1U << 27)
#define SPU_REV_MASK_mLAPF2      (1U << 28)
#define SPU_REV_MASK_mRAPF2      (1U << 29)
#define SPU_REV_MASK_vLIN        (1U << 30)
#define SPU_REV_MASK_vRIN        (1U << 31)

typedef struct {
    u32 m_Mask;
    ReverbRegisters m_Regs;
} ReverbPreset;

#define SPU_MIN_ADDR 0x1010
#define SPU_MAX_SIZE 512 * 1024
#define SPU_MAX_ALIGNED_ADDR (SPU_MAX_SIZE - 8) // 0x7fff8 - largest 8-byte aligned addr < 512KB
#define SPU_MAX_VALID_OFFSET (SPU_MAX_ALIGNED_ADDR - SPU_MIN_ADDR)  // 0x7efe8

#define SPU_DMA_CMD_READ 0
#define SPU_DMA_CMD_WRITE 1
#define SPU_DMA_CMD_SETADDR 2
#define SPU_DMA_CMD_EXEC 3

#define SPU_CONTROL_FLAG_CD_AUDIO_ENABLE    (1u <<  0)
#define SPU_CONTROL_FLAG_EXT_AUDIO_ENABLE   (1u <<  1)
#define SPU_CONTROL_FLAG_CD_AUDIO_REVERB    (1u <<  2)
#define SPU_CONTROL_FLAG_EXT_AUDIO_REVERB   (1u <<  3)
#define SPU_CONTROL_SRAM_TRANSFER_MODE      (1u <<  4) // 2 bits
#define SPU_CONTROL_FLAG_IRQ9_ENABLE        (1u <<  6)
#define SPU_CONTROL_FLAG_MASTER_REVERB      (1u <<  7)
#define SPU_CONTROL_NOISE_FREQUENCY_STEP    (1u <<  8) // 2 bits
#define SPU_CONTROL_NOISE_FREQUENCY_SHIFT   (1u << 10) // 4 bits
#define SPU_CONTROL_FLAG_MUTE_SPU           (1u << 14)
#define SPU_CONTROL_FLAG_SPU_ENABLE         (1u << 15)

extern long g_SpuRunning;
extern long g_SpuEVdma;
extern u_short _spu_tsa;
extern long _spu_mem_mode_plus;
extern volatile SpuIRQCallbackProc _spu_IRQCallback;
extern volatile SpuTransferCallbackProc _spu_transferCallback;
extern int _spu_dma_mode;
extern long g_SpuTransferMode;
extern long _spu_transMode;
extern long g_SpuReverbFlag;
extern long g_bSpuReserveWorkArea;
extern long g_SpuReverbOffsetAddress;
extern volatile long g_ReverbMode;
extern short g_ReverbVolumeLeft;
extern short g_ReverbVolumeRight;
extern long g_ReverbDelay;
extern long g_ReverbFeedback;
extern union SpuUnion* _spu_RXX;
extern volatile int* _spu_madr;
extern volatile int* _spu_bcr;
extern volatile int* _spu_chcr;
extern long _spu_inTransfer;
extern ReverbPreset g_ReverbParameterTable[SPU_REV_MODE_MAX];
extern volatile u16 _spu_RQ[10];

void _spu_FiDMA(void); // Forward declare for SpuStart()
s32 _spu_t(s32, ...); // Forward declare for _spu_Fr and _spu_Fw

void SpuInit(void) {
    _SpuInit(0);
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", _SpuInit);

void SpuStart(void) {
    s32 event;

    if (g_SpuRunning == 0) {
        g_SpuRunning = 1;
        EnterCriticalSection();
        _SpuDataCallback(_spu_FiDMA);
        // TODO: who knows what these constants do
        event = OpenEvent(0xF0000009U, 0x20, 0x2000, NULL);
        g_SpuEVdma = event;
        EnableEvent(event);
        ExitCriticalSection();
    }
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", _spu_init);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", _spu_FwriteByIO);

void _spu_FiDMA(void) {
    s32 i;

    if (_spu_dma_mode == 0)
    {
        _spu_Fw1ts();
    }

    _spu_RXX->rxx.spucnt &= ~0x30;

    for(i = 0; _spu_RXX->rxx.spucnt & 0x30; )
    {
        if (++i > 0xF00U) {
            break;
        }
    }

    if (_spu_transferCallback)
    {
        _spu_transferCallback();
    }
    else
    {
        DeliverEvent(0xF0000009, 0x20);
    }
}

void _spu_Fr_(s32 madr, u16 trans_addr, s32 bcr) {
    _spu_RXX->rxx.trans_addr = trans_addr;
    _spu_Fw1ts();
    _spu_RXX->rxx.spucnt |= 0x30;
    _spu_Fw1ts();
    _spu_FsetDelayR();
    *_spu_madr = madr;
    *_spu_bcr = (bcr << 0x10) | 0x10;
    _spu_dma_mode = 1;
    *_spu_chcr = 0x01000200;
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", _spu_t);

// TODO(jperos): Will need to revisit this for argument names after decompiling _spu_t to see what they are
s32 _spu_Fw(s32 arg0, s32 arg1) {
    if (_spu_transMode == SPU_TRANSFER_BY_DMA) {
        _spu_t(SPU_DMA_CMD_SETADDR, _spu_tsa << _spu_mem_mode_plus);
        _spu_t(SPU_DMA_CMD_WRITE);
        _spu_t(SPU_DMA_CMD_EXEC, arg0, arg1);
    }
    else
    {
        _spu_FwriteByIO(arg0, arg1);
    }
    return arg1;
}

s32 _spu_Fr(s32 arg0, s32 arg1) {
    _spu_t(SPU_DMA_CMD_SETADDR, _spu_tsa << _spu_mem_mode_plus);
    _spu_t(SPU_DMA_CMD_READ);
    _spu_t(SPU_DMA_CMD_EXEC, arg0, arg1);
    return arg1;
}

void _spu_FsetRXX(u32 offset, u32 value, u32 mode)
{
    if (mode == 0) {
        volatile u16* pRegs = (volatile u16*)_spu_RXX;
        pRegs[offset] = value;
    }
    else {
        volatile u16* pRegs = (volatile u16*)_spu_RXX;
        pRegs[offset] = value >> _spu_mem_mode_plus;
    }
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", _spu_FsetRXXa);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", _spu_FgetRXXa);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", _spu_FsetPCR);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", _spu_FsetDelayW);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", _spu_FsetDelayR);

void _spu_Fw1ts(void) {
    volatile s32 counter;
    volatile s32 value;
    
    value = 13;
    counter = 0;
    
    while (counter < 60) {
        value = value * 13;
        counter++;
    }
}

void _SpuDataCallback(SpuTransferCallbackProc func) {
    DMACallback(4, func);
}

void SpuQuit(void) {
    if (g_SpuRunning == 1) {
        g_SpuRunning = 0;
        EnterCriticalSection();
        _spu_transferCallback = NULL;
        _spu_IRQCallback = NULL;
        _SpuDataCallback(NULL);
        CloseEvent(g_SpuEVdma);
        DisableEvent(g_SpuEVdma);
        ExitCriticalSection();
    }
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", SpuInitMalloc);

long SpuSetNoiseClock(long n_clock) {
    long clamped;
    u16 spucnt;
    u16 newVal;
    

    if (n_clock < 0) {
        clamped = 0;
    } else if (n_clock > 0x3F) {  // >= 64
        clamped = 0x3F;      // = 63
    } else {
        clamped = n_clock;
    }
    
    spucnt = _spu_RXX->_rxx.spucnt;
    newVal = (spucnt & 0xC0FF) | ((clamped & 0x3F) << 8);
    _spu_RXX->rxx.spucnt = newVal;

    return clamped;
}

long SpuSetReverb (long on_off) // 100% matching on PSYQ4.0 (gcc 2.7.2 + aspsx 2.56)
{
    long spuControlRegister;

    switch( on_off ) {
    case SPU_OFF:
        g_SpuReverbFlag = SPU_OFF;
        spuControlRegister = _spu_RXX->_rxx.spucnt;
        spuControlRegister &= ~SPU_CONTROL_FLAG_MASTER_REVERB;
        _spu_RXX->_rxx.spucnt = spuControlRegister;
        break;
        
    case SPU_ON:
        if( g_bSpuReserveWorkArea != on_off && _SpuIsInAllocateArea_(g_SpuReverbOffsetAddress) ) {
            g_SpuReverbFlag = SPU_OFF;
            spuControlRegister = _spu_RXX->_rxx.spucnt;
            spuControlRegister &= ~SPU_CONTROL_FLAG_MASTER_REVERB;
            _spu_RXX->_rxx.spucnt = spuControlRegister;
        } else {
            g_SpuReverbFlag = on_off;
            spuControlRegister = _spu_RXX->_rxx.spucnt;
            spuControlRegister |= SPU_CONTROL_FLAG_MASTER_REVERB;
            _spu_RXX->_rxx.spucnt = spuControlRegister;
        }
        break;
    }

    return g_SpuReverbFlag;
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", _SpuIsInAllocateArea);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", _SpuIsInAllocateArea_);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", SpuReadDecodedData);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", SpuSetIRQ);

SpuIRQCallbackProc SpuSetIRQCallback(SpuIRQCallbackProc func) {
    SpuIRQCallbackProc callback = _spu_IRQCallback;
    if (func != callback ) {
        _spu_IRQCallback = func;
        _SpuCallback(func);
    }
    return callback;
}

void _SpuCallback(SpuIRQCallbackProc func) {
    InterruptCallback(9, func);
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", SpuGetVoiceEnvelopeAttr);

u_long SpuRead(u_char* addr, u_long size) {

    if (size > 0x7EFF0U) {
        size = 0x7EFF0;
    }
    _spu_Fr(addr, size);
    if (_spu_transferCallback == 0) {
        _spu_inTransfer = 0;
    }
    return size;
}

// Possible SpuWrite
INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", func_8004D878);

u_long SpuSetTransferStartAddr(u_long addr) {
    u32 offset;
    u16 base_addr;

    offset = addr - SPU_MIN_ADDR;
    if (offset > SPU_MAX_VALID_OFFSET) {
        return 0;
    }

    base_addr = _spu_FsetRXXa(-1, addr);
    _spu_tsa = base_addr;
    return (ulong)base_addr << _spu_mem_mode_plus;
}

long SpuSetTransferMode(long mode) {
    int value;

    switch (mode) {
    case SPU_TRANSFER_BY_DMA:
        value = 0;
        break;
    case SPU_TRANSFER_BY_IO:
        value = 1;
        break;
    default:
        value = 0;
    }
    g_SpuTransferMode = mode;
    _spu_transMode = value;
    return value;
}

SpuTransferCallbackProc SpuSetTransferCallback(SpuTransferCallbackProc func) {
    SpuTransferCallbackProc previousCallback = _spu_transferCallback;
    if (func != previousCallback) {
        _spu_transferCallback = func;
    }
    return previousCallback;
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", SpuSetCommonAttr);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", SpuSetReverbModeType);

void _spu_setReverbAttr(ReverbPreset* preset) {
    s32 Mask;
    s32 bSetAllAttribues;

    Mask = preset->m_Mask;
    bSetAllAttribues = Mask == 0;
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_dAPF1)) {
        _spu_RXX->_rxx.m_Reverb.m_dAPF1 = preset->m_Regs.m_dAPF1;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_dAPF2)) {
        _spu_RXX->_rxx.m_Reverb.m_dAPF2 = preset->m_Regs.m_dAPF2;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_vIIR)) {
        _spu_RXX->_rxx.m_Reverb.m_vIIR = preset->m_Regs.m_vIIR;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_vCOMB1)) {
        _spu_RXX->_rxx.m_Reverb.m_vCOMB1 = preset->m_Regs.m_vCOMB1;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_vCOMB2)) {
        _spu_RXX->_rxx.m_Reverb.m_vCOMB2 = preset->m_Regs.m_vCOMB2;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_vCOMB3)) {
        _spu_RXX->_rxx.m_Reverb.m_vCOMB3 = preset->m_Regs.m_vCOMB3;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_vCOMB4)) {
        _spu_RXX->_rxx.m_Reverb.m_vCOMB4 = preset->m_Regs.m_vCOMB4;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_vWALL)) {
        _spu_RXX->_rxx.m_Reverb.m_vWALL = preset->m_Regs.m_vWALL;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_vAPF1)) {
        _spu_RXX->_rxx.m_Reverb.m_vAPF1 = preset->m_Regs.m_vAPF1;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_vAPF2)) {
        _spu_RXX->_rxx.m_Reverb.m_vAPF2 = preset->m_Regs.m_vAPF2;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_mLSAME)) {
        _spu_RXX->_rxx.m_Reverb.m_mLSAME = preset->m_Regs.m_mLSAME;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_mRSAME)) {
        _spu_RXX->_rxx.m_Reverb.m_mRSAME = preset->m_Regs.m_mRSAME;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_mLCOMB1)) {
        _spu_RXX->_rxx.m_Reverb.m_mLCOMB1 = preset->m_Regs.m_mLCOMB1;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_mRCOMB1)) {
        _spu_RXX->_rxx.m_Reverb.m_mRCOMB1 = preset->m_Regs.m_mRCOMB1;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_mLCOMB2)) {
        _spu_RXX->_rxx.m_Reverb.m_mLCOMB2 = preset->m_Regs.m_mLCOMB2;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_mRCOMB2)) {
        _spu_RXX->_rxx.m_Reverb.m_mRCOMB2 = preset->m_Regs.m_mRCOMB2;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_dLSAME)) {
        _spu_RXX->_rxx.m_Reverb.m_dLSAME = preset->m_Regs.m_dLSAME;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_dRSAME)) {
        _spu_RXX->_rxx.m_Reverb.m_dRSAME = preset->m_Regs.m_dRSAME;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_mLDIFF)) {
        _spu_RXX->_rxx.m_Reverb.m_mLDIFF = preset->m_Regs.m_mLDIFF;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_mRDIFF)) {
        _spu_RXX->_rxx.m_Reverb.m_mRDIFF = preset->m_Regs.m_mRDIFF;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_mLCOMB3)) {
        _spu_RXX->_rxx.m_Reverb.m_mLCOMB3 = preset->m_Regs.m_mLCOMB3;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_mRCOMB3)) {
        _spu_RXX->_rxx.m_Reverb.m_mRCOMB3 = preset->m_Regs.m_mRCOMB3;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_mLCOMB4)) {
        _spu_RXX->_rxx.m_Reverb.m_mLCOMB4 = preset->m_Regs.m_mLCOMB4;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_mRCOMB4)) {
        _spu_RXX->_rxx.m_Reverb.m_mRCOMB4 = preset->m_Regs.m_mRCOMB4;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_dLDIFF)) {
        _spu_RXX->_rxx.m_Reverb.m_dLDIFF = preset->m_Regs.m_dLDIFF;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_dRDIFF)) {
        _spu_RXX->_rxx.m_Reverb.m_dRDIFF = preset->m_Regs.m_dRDIFF;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_mLAPF1)) {
        _spu_RXX->_rxx.m_Reverb.m_mLAPF1 = preset->m_Regs.m_mLAPF1;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_mRAPF1)) {
        _spu_RXX->_rxx.m_Reverb.m_mRAPF1 = preset->m_Regs.m_mRAPF1;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_mLAPF2)) {
        _spu_RXX->_rxx.m_Reverb.m_mLAPF2 = preset->m_Regs.m_mLAPF2;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_mRAPF2)) {
        _spu_RXX->_rxx.m_Reverb.m_mRAPF2 = preset->m_Regs.m_mRAPF2;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_vLIN)) {
        _spu_RXX->_rxx.m_Reverb.m_vLIN = preset->m_Regs.m_vLIN;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_vRIN)) {
        _spu_RXX->_rxx.m_Reverb.m_vRIN = preset->m_Regs.m_vRIN;
    }
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", SpuClearReverbWorkArea);

// Probable WaitEvent()
INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", func_8004E564);

void SpuSetReverbModeDepth(short DepthL, short DepthR)
{
    short* RL = &_spu_RXX->_rxx.rev_vol.left;
    short* RR = &_spu_RXX->_rxx.rev_vol.right;
    *RL = DepthL;
    *RR = DepthR;
    g_ReverbVolumeLeft = DepthL;
    g_ReverbVolumeRight = DepthR;
}

void SpuSetReverbModeDelayTime(long delayTime) {
    ReverbPreset preset;
    u8* pSrc;
    u8* pDst;
    s32 bytesToCopy;
    s32 scaledDelay;
    s32 scaledDelay2;

    s32 mode = g_ReverbMode;
    if (mode <= SPU_REV_MODE_DELAY)
    {
        if (SPU_REV_MODE_ECHO <= mode)
        {
            pDst = (u8*)&preset;
            bytesToCopy = sizeof(ReverbPreset) - 1;
            pSrc = (u8*)&g_ReverbParameterTable[g_ReverbMode];

            while (bytesToCopy != -1) {
                *pDst++ = *pSrc++;
                bytesToCopy--;
            };
            
            preset.m_Mask = SPU_REV_MASK_mLSAME | SPU_REV_MASK_mRSAME | SPU_REV_MASK_mLCOMB1 
                          | SPU_REV_MASK_dLSAME | SPU_REV_MASK_mLAPF1 | SPU_REV_MASK_mRAPF1;
            
            scaledDelay  = (delayTime << 12) / 127;  /* delayTime * 4096 / 127 */
            scaledDelay2 = (delayTime << 13) / 127;  /* delayTime * 8192 / 127 */
            
            preset.m_Regs.m_mLSAME  = scaledDelay2 - preset.m_Regs.m_dAPF1;
            preset.m_Regs.m_mRSAME  = scaledDelay  - preset.m_Regs.m_dAPF2;
            preset.m_Regs.m_mLCOMB1 = preset.m_Regs.m_mRCOMB1 + scaledDelay;
            preset.m_Regs.m_dLSAME  = preset.m_Regs.m_dRSAME + scaledDelay;
            preset.m_Regs.m_mLAPF1  = preset.m_Regs.m_mLAPF2 + scaledDelay;
            preset.m_Regs.m_mRAPF1  = preset.m_Regs.m_mRAPF2 + scaledDelay;
            g_ReverbDelay = delayTime;
            _spu_setReverbAttr(&preset);
        }
    }
}

void SpuSetReverbModeFeedback(long feedback) {
    ReverbPreset preset;
    u8* pSrc;
    u8* pDst;
    s32 bytesToCopy;
    s32 mode = g_ReverbMode;

    if (mode <= SPU_REV_MODE_DELAY) {
        if (SPU_REV_MODE_ECHO <= mode) {
            pDst = (u8*)&preset;
            bytesToCopy = sizeof(ReverbPreset) - 1;
            pSrc = (u8*)&g_ReverbParameterTable[g_ReverbMode];
            
            while (bytesToCopy != -1) {
                *pDst++ = *pSrc++;
                bytesToCopy--;
            };
            
            g_ReverbFeedback = feedback;
            preset.m_Mask = SPU_REV_MASK_vWALL;
            preset.m_Regs.m_vWALL = (feedback * 0x8100) / 127;
            _spu_setReverbAttr(&preset);
        }
    }
}

void SpuGetReverbModeType(long* type)
{
    *type = g_ReverbMode;
}