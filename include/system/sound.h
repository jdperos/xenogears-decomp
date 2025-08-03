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


#define SOUND_STATUS_OK 0x0
#define SOUND_ERR_INVALID_SIGNATURE 0x1
#define SOUND_ERR_INVALID_CHECKSUM 0x2 // Maybe
#define SOUND_ERR_UNK_0X4 0x4
#define SOUND_ERR_ENTRY_ALREADY_EXISTS 0x15

#define FILE_SIGNATURE(a, b, c, d) (d<<24)+(c<<16)+(b<<8)+a

// Possibly a struct which can either be a SMD (Background Music), SED (Sound Effect) or SND entry
struct SoundFile_t {
    /* 0x0 */ unsigned int magic;
    /* 0x4 */ u32 unk4;
    /* 0x8 */ u32 unk8; // Size?
    /* 0xC */ u16 unkC; // File format version?
    /* 0xE */ u16 unkE;
    /* 0x10 */ u32 unk10; // smdId?
    /* 0x14 */ unsigned short sedId;
    /* 0x16 */ unsigned short sndId;
    /* 0x18 */ u32 unk18;
    /* 0x1C */ struct SoundFile_t* pNext;
    /* 0x20 */ // starts of 0x2 size offsets to scripts. Pair of scripts for each instrument. 1 script for 1 channel.
};
typedef struct SoundFile_t SoundFile;

extern long D_800595BC; // Unknown event descriptor
extern SoundFile* g_SoundSedsLinkedList;

extern void* g_pSoundSpuRegisters;

extern short g_SoundSpuErrorId;
extern short g_SoundControlFlags;
extern SpuIRQCallbackProc g_SoundSpuIrqCallbackFn;
extern int g_SoundSpuIRQCount;

extern SoundVoiceData g_SoundVoiceDataPointerArray;

extern SpuVolume g_SoundUnkVolume;

extern CdlATV g_SoundCdRomAttenuation;

#endif