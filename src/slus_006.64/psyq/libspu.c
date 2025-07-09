#include "common.h"
#include "psyq/libspu.h"

typedef struct {
    u32 Data1; // Volume
    u32 Data2; // Pitch/loop
    u32 Data3; // ADSR
    u32 Data4; // Misc
} VoiceData;

// TODO(jperos): Fill out this big ol struct
typedef struct {
    VoiceData voiceData[24];
    u8 padding[0x2A];
    u16 controlRegister;
} SpuRegisters;

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

extern long g_SpuReverbFlag;
extern long g_bSpuReserveWorkArea;
extern long g_SpuReverbOffsetAddress;
extern SpuRegisters* g_pSpuRegisters;

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", _spu_t);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", func_8004CF38);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", func_8004CFC0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", func_8004D028);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", func_8004D070);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", func_8004D114);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", func_8004D150);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", func_8004D1B0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", func_8004D1DC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", func_8004D208);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", func_8004D270);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", func_8004D294);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", SpuInitMalloc);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", func_8004D364);

// INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", SpuSetReverb);
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

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", func_8004D7A8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", func_8004D818);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", func_8004D878);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", func_8004D8D8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", func_8004D930);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", SpuSetTransferCallback);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", SpuSetCommonAttr);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", func_8004DD1C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", func_8004DEF8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", SpuClearReverbWorkArea);
