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

// TODO(jperos): Fill out this big ol struct
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

    // Memory
    u16 m_ReverbWorkStartAddr;
    u16 m_IrqAddress;
    u16 m_TransferAddress;
    u16 m_TransferData;
    u16 m_TransferControl;

    // Control
    u16 controlRegister;
    u16 m_StatusRegister;

    // Aux volumes
    s16 m_CdInputVolumeL;
    s16 m_CdInputVolumeR;
    s16 m_ExtInputVolumeL;
    s16 m_ExtInputVolumeR;

    ReverbRegisters m_Reverb;
} SpuRegisters;

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
extern long g_ReverbMode;
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

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", _spu_setReverbAttr);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", SpuClearReverbWorkArea);

// Probable WaitEvent()
INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", func_8004E564);

// TODO(jperos): This one should be very easy once I fill out the SpuRegisters struct
INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", SpuSetReverbModeDepth);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", SpuSetReverbModeDelayTime);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", SpuSetReverbModeFeedback);

void SpuGetReverbModeType(long* type)
{
    *type = g_ReverbMode;
}