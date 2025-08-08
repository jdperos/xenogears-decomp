#ifndef _XENO_SOUND_INPUT_H
#define _XENO_SOUND_INPUT_H

#include "psyq/libspu.h"
#include "psyq/libcd.h"

#define NUM_VOICES 24

extern SpuCommonAttr g_SoundSpuCommonAttr;

typedef struct {
    s16 assignedVoice;
    u16 modeFlags;
    s16 priority;
    u16 flags;
    SpuVolume volume;
    u8 unkC[8];
    u16 pitch;
    u8 unk16[6];
    u32 startAddress;
    u32 loopAddress;
    u8 unkAdsr1;
    u8 unkAdsr2;
    u8 unkAdsr3;
    u8 unkAdsr4;
    u8 adsrDR;
    u8 unkAdsr5;
    u8 unkAdsr6;
    u8 adsrSR;
} SoundVoiceData;

#define SOUND_CTL_FLAG_IRQ_HANDLER (1 << 2)

#define SOUND_STATUS_OK 0x0
#define SOUND_ERR_INVALID_SIGNATURE 0x1
#define SOUND_ERR_INVALID_CHECKSUM 0x2 // Maybe
#define SOUND_ERR_UNK_0X4 0x4
#define SOUND_ERR_ENTRY_ALREADY_EXISTS 0x15
#define SOUND_ERR_UNEXPECTED_CALLBACK 0x26

#define SOUND_TRANSFER_QUEUE_SIZE 8

#define SOUND_SPU_COMMAND_WRITE 0x1
#define SOUND_SPU_COMMAND_READ 0x2

#define SOUND_WDS_ALLOCATE_AT_ADDRESS 0
#define SOUND_WDS_ALLOCATE_AUTOMATIC -1

#define FILE_SIGNATURE(a, b, c, d) (d<<24)+(c<<16)+(b<<8)+a

typedef void (*SoundCommandCallback_t)(void);

typedef struct {
    /* 0x0 */ u16 unk0; // Flags?
    /* 0x2 */ u16 unk2;
    /* 0x4 */ u32 unk4;
    /* 0x8 */ void* pPrev;
    /* 0xC */ void* pNext;
} SoundHeapBlockHeader;


#define MAX_SPU_MEMORY_BLOCKS 0xC

#define SPU_MEMORY_FREE 0x0
#define SPU_MEMORY_RESERVED 0x1 // Or last block?
#define SPU_MEMORY_IN_USE 0x80

typedef struct {
    /* 0x0 */ unsigned char flags;
    /* 0x1 */ unsigned char unk1; // Type of memory/data?
    /* 0x2 */ short nextBlockIndex;
    /* 0x4 */ int spuAddress;
    /* 0x8 */ int size;
    /* 0xC */ unsigned int unkC; // Padding?
} SoundSpuMemoryBlock;

// Possible a more general queue to sending commands to the SPU,
// but all supported commands has to do with data transfer.
typedef struct {
    /* 0x0  */ u_short commandType;
    /* 0x2  */ short unk2;
    /* 0x4  */ void* pSpuData;
    /* 0x8  */ u32 pTransferAddress;
    /* 0xC  */ u_long dataSize;
    /* 0x10 */ SoundCommandCallback_t pCallbackFn;
} SoundTransferCommand;

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

struct SoundWDSEntry_t {
    /* 0x0  */ u_char _unk0[0x10];
    /* 0x10 */ unsigned int headerSizeMby;
    /* 0x14 */ unsigned int adpcmDataSize; // Sample size
    /* 0x18 */ unsigned int adpcmDataOffset; // Offset to data to write to SPU
    /* 0x1C */ unsigned short unk1C;
    /* 0x1E */ unsigned short unk1E;
    /* 0x20 */ unsigned short id;
    /* 0x22 */ u_short unk22;
    /* 0x24 */ u_int unk24;
    /* 0x28 */ int spuMemoryAddress; // Optional
    /* 0x2C */ struct SoundWDSEntry_t* pNext;
};
typedef struct SoundWDSEntry_t SoundWDSEntry;


typedef struct {
    u16 active_flag;                // 0x00 - checked for != 0
    u16 status_flags;               // 0x02 - contains bit flags of some sort
    s8 unknown_data[0x23];          // 0x04 - 0x26
    u8 voice_number;                // 0x27 - SPU voice index
    s8 padding[0x8];                // 0x28 - 0x2F
    SoundVoiceData voice_data;      // 0x30 - passed to cancel function
    u8 unk[252]                     // Size of this struct during iteration seems to be 0x158
} AudioElement;

typedef struct {
    // Manager header (0x00 - 0x93)
    s8 header_data[0x14];
    u8 element_count;             // 0x14
    s8 more_header[0x7F];         // 0x15 - 0x93
    AudioElement elements[24];    // Array of audio elements starts at 0x94 (no idea that it has to be 24)
} AudioManager;

// Heap
extern SoundHeapBlockHeader* g_SoundHeapHead;
extern u32 g_SoundHeapEnd;
extern u32 g_SoundHeapSize;

// SPU Transfer Command Queue
extern s16 D_80059548;
extern SoundTransferCommand* g_SoundTransferQueue;
extern u16 g_SoundTransferQueueReadIndex;
extern u16 g_SoundTransferQueueWriteIndex;

// SPU Memory Management
extern SoundSpuMemoryBlock g_SoundSpuMemoryBlocks[MAX_SPU_MEMORY_BLOCKS];

extern u_long g_unk_SoundEvent; // Event Descriptor

extern SoundFile* g_SoundSedsLinkedList;
extern SoundWDSEntry* g_SoundWdsLinkedList;

extern int g_SoundWdsCurSpuAddress;
extern int g_SoundWdsRemainingBytes;

extern void* g_pSoundSpuRegisters;

extern short g_SoundSpuErrorId;
extern long g_unk_VoicesNeedingProcessing;
extern short g_SoundControlFlags;
extern SpuIRQCallbackProc g_SoundSpuIrqCallbackFn;
extern int g_SoundSpuIRQCount;

extern u32 g_SoundKeyOnFlags;
extern u32 g_SoundKeyOffFlags;
extern SoundVoiceData* g_SoundChannels[24];

extern SpuVolume g_SoundUnkVolume;

extern CdlATV g_SoundCdRomAttenuation;


#endif