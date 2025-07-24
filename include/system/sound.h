#ifndef _XENO_SOUND_INPUT_H
#define _XENO_SOUND_INPUT_H

#include "psyq/libspu.h"
#include "psyq/libcd.h"

#define NUM_VOICES 24

extern SpuCommonAttr g_SoundSpuCommonAttr;

typedef struct {
    u8 m_Unk0[2];
    u16 m_ModeFlags;
    u8 m_Unk4[2];
    u16 m_Flags;
    s16 m_VolumeL;
    s16 m_VolumeR;
    u8 m_UnkC[8];
    u16 m_Pitch;
    u8 Unk16[6];
    u32 m_StartAddress;
    u32 m_LoopAddress;
    u8 m_UnkAdsr1;
    u8 m_UnkAdsr2;
    u8 m_UnkAdsr3;
    u8 m_UnkAdsr4;
    u8 m_AdsrDR;
    u8 m_UnkAdsr5;
    u8 m_UnkAdsr6;
    u8 m_AdsrSR;
} SoundVoiceData;

extern SoundVoiceData g_SoundVoiceDataPointerArray;

extern SpuVolume g_SoundUnkVolume;

extern CdlATV g_SoundCdRomAttenuation;

#endif