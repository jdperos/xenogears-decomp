#include "common.h"
#include "psyq/libspu.h"

typedef struct {
    u16 m_VolumeL;
    u16 m_VolumeR;
    u16 m_SampleRate;
    u16 m_StartAddress;
    u16 m_Adsr1;
    u16 m_Adsr2;
    u16 m_CurrentAdsrVol;
    u16 m_LoopAddress;
} VoiceData;

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
    VoiceData voiceData[24];
    // Volumes
    s16 m_MainVolumeL; // 1-bit for Volume Mode, 15-bits for Volume
    s16 m_MainVolumeR; // 1-bit for Volume Mode, 15-bits for Volume
    s16 m_ReverbOutVolumeL; // Full 16 bits for volume
    s16 m_ReverbOutVolumeR; // Full 16 bits for volume

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
    u16 m_TransferAddress;
    u16 m_TransferFifo;

    // Control
    u16 controlRegister;
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
} SpuRegisters;

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

extern long g_SpuTransferMode;
extern long g_SpuReverbFlag;
extern long g_bSpuReserveWorkArea;
extern long g_SpuReverbOffsetAddress;
extern volatile long g_ReverbMode;
extern short g_ReverbVolumeLeft;
extern short g_ReverbVolumeRight;
extern long g_ReverbDelay;
extern long g_ReverbFeedback;
extern SpuRegisters* g_pSpuRegisters;
extern ReverbPreset g_ReverbParameterTable[SPU_REV_MODE_MAX];

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", _spu_t);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", func_8004CF38);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", func_8004CFC0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", func_8004D028);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", _spu_FsetRXXa);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", func_8004D114);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", func_8004D150);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", func_8004D1B0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", func_8004D1DC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", func_8004D208);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", func_8004D270);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", func_8004D294);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", SpuInitMalloc);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", SpuSetNoiseClock);

long SpuSetReverb (long on_off) // 100% matching on PSYQ4.0 (gcc 2.7.2 + aspsx 2.56)
{
    long spuControlRegister;

    switch( on_off ) {
    case SPU_OFF:
        g_SpuReverbFlag = SPU_OFF;
        spuControlRegister = g_pSpuRegisters->controlRegister;
        spuControlRegister &= ~SPU_CONTROL_FLAG_MASTER_REVERB;
        g_pSpuRegisters->controlRegister = spuControlRegister;
        break;
        
    case SPU_ON:
        if( g_bSpuReserveWorkArea != on_off && _SpuIsInAllocateArea_(g_SpuReverbOffsetAddress) ) {
            g_SpuReverbFlag = SPU_OFF;
            spuControlRegister = g_pSpuRegisters->controlRegister;
            spuControlRegister &= ~SPU_CONTROL_FLAG_MASTER_REVERB;
            g_pSpuRegisters->controlRegister = spuControlRegister;
        } else {
            g_SpuReverbFlag = on_off;
            spuControlRegister = g_pSpuRegisters->controlRegister;
            spuControlRegister |= SPU_CONTROL_FLAG_MASTER_REVERB;
            g_pSpuRegisters->controlRegister = spuControlRegister;
        }
        break;
    }

    return g_SpuReverbFlag;
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", func_8004D484);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", _SpuIsInAllocateArea_);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", func_8004D590);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", SpuSetIRQ);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", SpuSetIRQCallback);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", func_8004D784);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", SpuGetVoiceEnvelopeAttr);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", func_8004D818);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", func_8004D878);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", func_8004D8D8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", SpuSetTransferMode);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", SpuSetTransferCallback);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", SpuSetCommonAttr);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", SpuSetReverbModeType);

void _spu_setReverbAttr(ReverbPreset* preset) {
    s32 Mask;
    s32 bSetAllAttribues;

    Mask = preset->m_Mask;
    bSetAllAttribues = Mask == 0;
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_dAPF1)) {
        g_pSpuRegisters->m_Reverb.m_dAPF1 = preset->m_Regs.m_dAPF1;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_dAPF2)) {
        g_pSpuRegisters->m_Reverb.m_dAPF2 = preset->m_Regs.m_dAPF2;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_vIIR)) {
        g_pSpuRegisters->m_Reverb.m_vIIR = preset->m_Regs.m_vIIR;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_vCOMB1)) {
        g_pSpuRegisters->m_Reverb.m_vCOMB1 = preset->m_Regs.m_vCOMB1;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_vCOMB2)) {
        g_pSpuRegisters->m_Reverb.m_vCOMB2 = preset->m_Regs.m_vCOMB2;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_vCOMB3)) {
        g_pSpuRegisters->m_Reverb.m_vCOMB3 = preset->m_Regs.m_vCOMB3;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_vCOMB4)) {
        g_pSpuRegisters->m_Reverb.m_vCOMB4 = preset->m_Regs.m_vCOMB4;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_vWALL)) {
        g_pSpuRegisters->m_Reverb.m_vWALL = preset->m_Regs.m_vWALL;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_vAPF1)) {
        g_pSpuRegisters->m_Reverb.m_vAPF1 = preset->m_Regs.m_vAPF1;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_vAPF2)) {
        g_pSpuRegisters->m_Reverb.m_vAPF2 = preset->m_Regs.m_vAPF2;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_mLSAME)) {
        g_pSpuRegisters->m_Reverb.m_mLSAME = preset->m_Regs.m_mLSAME;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_mRSAME)) {
        g_pSpuRegisters->m_Reverb.m_mRSAME = preset->m_Regs.m_mRSAME;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_mLCOMB1)) {
        g_pSpuRegisters->m_Reverb.m_mLCOMB1 = preset->m_Regs.m_mLCOMB1;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_mRCOMB1)) {
        g_pSpuRegisters->m_Reverb.m_mRCOMB1 = preset->m_Regs.m_mRCOMB1;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_mLCOMB2)) {
        g_pSpuRegisters->m_Reverb.m_mLCOMB2 = preset->m_Regs.m_mLCOMB2;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_mRCOMB2)) {
        g_pSpuRegisters->m_Reverb.m_mRCOMB2 = preset->m_Regs.m_mRCOMB2;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_dLSAME)) {
        g_pSpuRegisters->m_Reverb.m_dLSAME = preset->m_Regs.m_dLSAME;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_dRSAME)) {
        g_pSpuRegisters->m_Reverb.m_dRSAME = preset->m_Regs.m_dRSAME;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_mLDIFF)) {
        g_pSpuRegisters->m_Reverb.m_mLDIFF = preset->m_Regs.m_mLDIFF;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_mRDIFF)) {
        g_pSpuRegisters->m_Reverb.m_mRDIFF = preset->m_Regs.m_mRDIFF;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_mLCOMB3)) {
        g_pSpuRegisters->m_Reverb.m_mLCOMB3 = preset->m_Regs.m_mLCOMB3;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_mRCOMB3)) {
        g_pSpuRegisters->m_Reverb.m_mRCOMB3 = preset->m_Regs.m_mRCOMB3;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_mLCOMB4)) {
        g_pSpuRegisters->m_Reverb.m_mLCOMB4 = preset->m_Regs.m_mLCOMB4;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_mRCOMB4)) {
        g_pSpuRegisters->m_Reverb.m_mRCOMB4 = preset->m_Regs.m_mRCOMB4;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_dLDIFF)) {
        g_pSpuRegisters->m_Reverb.m_dLDIFF = preset->m_Regs.m_dLDIFF;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_dRDIFF)) {
        g_pSpuRegisters->m_Reverb.m_dRDIFF = preset->m_Regs.m_dRDIFF;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_mLAPF1)) {
        g_pSpuRegisters->m_Reverb.m_mLAPF1 = preset->m_Regs.m_mLAPF1;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_mRAPF1)) {
        g_pSpuRegisters->m_Reverb.m_mRAPF1 = preset->m_Regs.m_mRAPF1;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_mLAPF2)) {
        g_pSpuRegisters->m_Reverb.m_mLAPF2 = preset->m_Regs.m_mLAPF2;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_mRAPF2)) {
        g_pSpuRegisters->m_Reverb.m_mRAPF2 = preset->m_Regs.m_mRAPF2;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_vLIN)) {
        g_pSpuRegisters->m_Reverb.m_vLIN = preset->m_Regs.m_vLIN;
    }
    if (bSetAllAttribues || (Mask & SPU_REV_MASK_vRIN)) {
        g_pSpuRegisters->m_Reverb.m_vRIN = preset->m_Regs.m_vRIN;
    }
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", SpuClearReverbWorkArea);

// Probable WaitEvent()
INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", func_8004E564);

void SpuSetReverbModeDepth(short DepthL, short DepthR)
{
    short* RL = &g_pSpuRegisters->m_ReverbOutVolumeL;
    short* RR = &g_pSpuRegisters->m_ReverbOutVolumeR;
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