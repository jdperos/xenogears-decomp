#ifndef _LIBSPU_INTERNAL_H
#define _LIBSPU_INTERNAL_H

#include "psyq/libspu.h"

typedef struct tagSpuMalloc {
    u32 addr;
    u32 size;
} SPU_MALLOC;

typedef struct {
    SpuVolume volume;
    u16 pitch;
    u16 addr;
    u16 adsr[2];
    u16 volumex;
    u16 loop_addr;
} SPU_VOICE_REG;

#define SPU_VOICE_REG_VOLUME_L    0
#define SPU_VOICE_REG_VOLUME_R    1
#define SPU_VOICE_REG_PITCH       2
#define SPU_VOICE_REG_ADDR        3
#define SPU_VOICE_REG_ADSR1       4
#define SPU_VOICE_REG_ADSR2       5
#define SPU_VOICE_REG_VOLUMEX     6
#define SPU_VOICE_REG_LOOP_ADDR   7
#define SPU_VOICE_REG_SIZE        8
#define NUM_VOICES               24

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
    SPU_VOICE_REG voice[NUM_VOICES];
    // Volumes
    SpuVolume main_vol; // 1-bit for Volume Mode, 15-bits for Volume
    SpuVolume rev_vol; // Full 16 bits for volume

    // Voice Flags
    u16 key_on[2];
    u16 key_off[2];
    u16 chan_fm[2];
    u16 noise_mode[2];
    u16 rev_mode[2];
    u32 m_EndxFlags;

    u16 unk;

    // Memory
    u16 rev_work_addr;
    u16 irq_addr;
    u16 trans_addr;
    u16 trans_fifo;

    // Control
    u16 spucnt;
    u16 data_trans;
    u16 spustat;

    // Aux volumes
    SpuVolume cd_vol;
    SpuVolume ex_vol;

    SpuVolume main_volx;

    u32 unk2;

    ReverbRegisters m_Reverb;
} SPU_RXX;

// Voice Registers (0x00 - 0xBF)
#define SPU_RXX_VOICE_BASE              0x00    // Voice registers start (24 voices × 8 regs each)
#define SPU_RXX_VOICE_SIZE              8       // Registers per voice
#define SPU_RXX_VOICE_END               0xBF    // Last voice register
// Global Volume Registers (0xC0 - 0xC3)
#define SPU_RXX_MAIN_VOL_L              0xC0    // Main volume left
#define SPU_RXX_MAIN_VOL_R              0xC1    // Main volume right
#define SPU_RXX_REV_VOL_L               0xC2    // Reverb volume left
#define SPU_RXX_REV_VOL_R               0xC3    // Reverb volume right
// Voice Flag Registers (0xC4 - 0xCF) - 32-bit values stored as pairs
#define SPU_RXX_KEY_ON_LOW              0xC4    // Key on flags (bits 0-15)
#define SPU_RXX_KEY_ON_HIGH             0xC5    // Key on flags (bits 16-31)
#define SPU_RXX_KEY_OFF_LOW             0xC6    // Key off flags (bits 0-15)
#define SPU_RXX_KEY_OFF_HIGH            0xC7    // Key off flags (bits 16-31)
#define SPU_RXX_PITCH_MOD_LOW           0xC8    // Pitch modulation flags (bits 0-15)
#define SPU_RXX_PITCH_MOD_HIGH          0xC9    // Pitch modulation flags (bits 16-31)
#define SPU_RXX_NOISE_LOW               0xCA    // Noise flags (bits 0-15)
#define SPU_RXX_NOISE_HIGH              0xCB    // Noise flags (bits 16-31)
#define SPU_RXX_REVERB_LOW              0xCC    // Reverb flags (bits 0-15)
#define SPU_RXX_REVERB_HIGH             0xCD    // Reverb flags (bits 16-31)
#define SPU_RXX_ENDX_LOW                0xCE    // End flags (bits 0-15)
#define SPU_RXX_ENDX_HIGH               0xCF    // End flags (bits 16-31)
// Memory Address Registers (0xD0 - 0xD4)
#define SPU_RXX_UNKNOWN_D0              0xD0    // Unknown register
#define SPU_RXX_REV_WA_START_ADDR       0xD1    // Reverb work area start address
#define SPU_RXX_IRQ_ADDR                0xD2    // IRQ address
#define SPU_RXX_TRANS_ADDR              0xD3    // Transfer address
#define SPU_RXX_TRANS_FIFO              0xD4    // Transfer FIFO
// Control Registers (0xD5 - 0xD7)
#define SPU_RXX_SPUCNT                  0xD5    // SPU control register
#define SPU_RXX_TRANS_CTRL              0xD6    // Transfer control
#define SPU_RXX_SPUSTAT                 0xD7    // SPU status register
// Audio Input Volume Registers (0xD8 - 0xDB)
#define SPU_RXX_CD_VOL_L                0xD8    // CD input volume left
#define SPU_RXX_CD_VOL_R                0xD9    // CD input volume right
#define SPU_RXX_EXT_VOL_L               0xDA    // External input volume left
#define SPU_RXX_EXT_VOL_R               0xDB    // External input volume right
// Current Volume Registers (0xDC - 0xDD)
#define SPU_RXX_CURR_MAIN_VOL_L         0xDC    // Current main volume left
#define SPU_RXX_CURR_MAIN_VOL_R         0xDD    // Current main volume right
// Unknown Register (0xDE - 0xDF)
#define SPU_RXX_UNKNOWN2_LOW            0xDE    // Unknown register (bits 0-15)
#define SPU_RXX_UNKNOWN2_HIGH           0xDF    // Unknown register (bits 16-31)
// Reverb Registers start at 0xE0
#define SPU_RXX_REVERB_BASE             0xE0    // Reverb parameter registers start

// SPU Control Register (SPUCNT) bit masks
#define SPU_CTRL_MASK_CD_AUDIO_ENABLE        (1 <<  0)              // 0
#define SPU_CTRL_MASK_EXT_AUDIO_ENABLE       (1 <<  1)              // 1
#define SPU_CTRL_MASK_CD_AUDIO_REVERB        (1 <<  2)              // 2
#define SPU_CTRL_MASK_EXT_AUDIO_REVERB       (1 <<  3)              // 3
#define SPU_CTRL_MASK_SRAM_TRANSFER_MODE    ((1 <<  4) | (1 << 5))  // 4-5
#define SPU_CTRL_MASK_IRQ9_ENABLE            (1 <<  6)              // 6
#define SPU_CTRL_MASK_REVERB_MASTER_ENABLE   (1 <<  7)              // 7
#define SPU_CTRL_MASK_NOISE_FREQ_STEP       ((1 <<  8) | (1 << 9))  // 8-9
#define SPU_CTRL_MASK_NOISE_FREQ_SHIFT      ((1 << 10) | (1 << 11) | (1 << 12) | (1 << 13))  // 10-13
#define SPU_CTRL_MASK_MUTE_SPU               (1 << 14)              // 14
#define SPU_CTRL_MASK_SPU_ENABLE             (1 << 15)              // 15

// SPU Control Register shift amounts for multi-bit fields
#define SPU_CTRL_SRAM_TRANSFER_SHIFT     4
#define SPU_CTRL_NOISE_FREQ_STEP_SHIFT   8
#define SPU_CTRL_NOISE_FREQ_SHIFT_SHIFT 10

#define SPU_CTRL_TRANSFER_MODE_STOP         ( 0 << SPU_CTRL_SRAM_TRANSFER_SHIFT ) // 0x00
#define SPU_CTRL_TRANSFER_MODE_MANUAL_WRITE ( 1 << SPU_CTRL_SRAM_TRANSFER_SHIFT ) // 0x10
#define SPU_CTRL_TRANSFER_MODE_DMA_WRITE    ( 2 << SPU_CTRL_SRAM_TRANSFER_SHIFT ) // 0x20
#define SPU_CTRL_TRANSFER_MODE_DMA_READ     ( 3 << SPU_CTRL_SRAM_TRANSFER_SHIFT ) // 0x30

// SPU Status Register (SPUSTAT) bit masks
#define SPU_STAT_MASK_CURRENT_SPU_MODE      ((1 <<  0) | (1 << 1) | (1 << 2) | (1 << 3) | (1 <<  4) | (1 <<  5))  // 0-5
#define SPU_STAT_MASK_IRQ9_FLAG              (1 <<  6)              // 6
#define SPU_STAT_MASK_DMA_READ_WRITE_REQUEST (1 <<  7)              // 7
#define SPU_STAT_MASK_DMA_WRITE_REQUEST      (1 <<  8)              // 8
#define SPU_STAT_MASK_DMA_READ_REQUEST       (1 <<  9)              // 9
#define SPU_STAT_MASK_DATA_TRANSFER_BUSY     (1 << 10)              // 10
#define SPU_STAT_MASK_CAPTURE_BUFFER_HALF    (1 << 11)              // 11
#define SPU_STAT_MASK_UNKNOWN_UNUSED        ((1 << 12) | (1 << 13) | (1 << 14) | (1 << 15))  // 12-15

// SPU Status Register shift amounts for multi-bit fields
#define SPU_STAT_CURRENT_SPU_MODE_SHIFT      0
#define SPU_STAT_UNKNOWN_UNUSED_SHIFT        12

// SPU Status Register values
#define SPU_STAT_CAPTURE_FIRSTHALF           (0 << 11)  // Writing to first half
#define SPU_STAT_CAPTURE_SECONDHALF          (1 << 11)  // Writing to second half
#define SPU_STAT_TRANSFER_READY              (0 << 10)  // Transfer ready
#define SPU_STAT_TRANSFER_BUSY               (1 << 10)  // Transfer busy

#define SPU_DMA_MODE_WRITE 0
#define SPU_DMA_MODE_READ  1

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

#define SPU_RAM_SIZE                   512 * 1024 // 512KB of SPU RAM
#define _SPU_RAM_SIZE                  (SPU_RAM_SIZE >> 3) // SPU-ranged address
#define SPU_DECODED_DATA_AREA_START    0x0
#define SPU_DECODED_DATA_AREA_SIZE     0x1000  // 4KB decoded data area 0x000-0xFFF
#define SPU_SYSTEM_RESERVED_AREA_START SPU_DECODED_DATA_AREA_SIZE // 16 bytes system area 0x1000-0x100F
#define SPU_SYSTEM_RESERVED_AREA_SIZE  0x10 // 16 bytes system area 0x1000-0x100F
#define SPU_RESERVED_TOTAL             (SPU_DECODED_DATA_AREA_SIZE + SPU_SYSTEM_RESERVED_AREA_SIZE) // 0x1010
#define SPU_DEFAULT_ADDR               SPU_SYSTEM_RESERVED_AREA_START
#define _SPU_DEFAULT_ADDR              (SPU_DEFAULT_ADDR >> 3)
#define SPU_MIN_ADDR                   SPU_RESERVED_TOTAL // Writing of samples starts after reserved area at 0x1010
#define SPU_MAX_ALIGNED_ADDR           (SPU_RAM_SIZE - 8) // 0x7fff8 - largest 8-byte aligned addr < 512KB
#define SPU_MAX_VALID_OFFSET           (SPU_MAX_ALIGNED_ADDR - SPU_MIN_ADDR)  // 0x7efe8
#define SPU_MAX_TRANSFER_SIZE          (SPU_RAM_SIZE - SPU_RESERVED_TOTAL) // 0x7EFF0

#define SPU_DMA_CMD_READ 0
#define SPU_DMA_CMD_WRITE 1
#define SPU_DMA_CMD_SETADDR 2
#define SPU_DMA_CMD_EXEC 3

#define SPU_ADDRESS_MODE_SPU (-1)
#define SPU_ADDRESS_MODE_ALIGNED (-2)

// TODO(jperos): This all compiles, but I am currently unsure as to the significance of these flags. Current best guesses here
#define SPU_MALLOC_UNK_FLAG_1      (1 << 28)
#define SPU_MALLOC_UNK_FLAG_2      (1 << 29)
#define SPU_MALLOC_END_MARKER      (1 << 30)
#define SPU_MALLOC_SKIP_ENTRY      (1 << 31)
#define SPU_MALLOC_FLAGS           (SPU_MALLOC_UNK_FLAG_1 | SPU_MALLOC_UNK_FLAG_2 | SPU_MALLOC_END_MARKER | SPU_MALLOC_SKIP_ENTRY)
#define SPU_MALLOC_ADDR_MASK       (~SPU_MALLOC_FLAGS)


// rodata
extern const char D_8001946C[]; // "SPU:T/O [%s]\n"
extern const char D_8001947C[]; // "wait (reset)"
extern const char D_8001948C[]; // "wait (wrdy H -> L)"
extern const char D_800194A0[]; // "wait (dmaf clear/W)"
extern const char D_800194B4[]; // "SPU:T/O [%s]\n"
extern const char D_800194C4[]; // "wait (IRQ/ON)"
extern const char D_800194D4[]; // "wait (IRQ/OFF)"

extern long _spu_EVdma;
extern u_long _spu_keystat;
extern long _spu_trans_mode;
extern long _spu_rev_flag;
extern long _spu_rev_reserve_wa;
extern long _spu_rev_offsetaddr;
extern SpuReverbAttr _spu_rev_attr;
extern long _spu_RQvoice;
extern volatile long _spu_RQmask;
extern u_short _spu_voice_centerNote[NUM_VOICES];
extern char _spu_zerobuf[1024];
extern long _spu_env;
extern long g_SpuRunning;
extern union SpuUnion* _spu_RXX;
extern volatile long* _spu_madr;
extern volatile long* _spu_bcr;
extern volatile long* _spu_chcr;
extern volatile long* _spu_delay;
extern volatile long* _spu_sys_pcr;
extern u_short _spu_tsa;
extern long _spu_transMode;
extern long _spu_addrMode;
extern long _spu_mem_mode;
extern long _spu_mem_mode_plus;
extern long _spu_mem_mode_unit;
extern long _spu_mem_mode_unitM;
extern long _spu_inTransfer;
extern volatile SpuTransferCallbackProc _spu_transferCallback;
extern volatile SpuIRQCallbackProc _spu_IRQCallback;
extern u_char _spu_dummy[16];
extern long _spu_dma_mode;
extern long _spu_transfer_startaddr;
extern long _spu_transfer_time;
extern long _spu_AllocBlockNum;
extern long _spu_AllocLastNum;
extern char* _spu_memList;
extern long _spu_rev_startaddr[];
extern ReverbPreset _spu_rev_param[SPU_REV_MODE_MAX];
extern volatile u_short _spu_RQ[10];

static inline void _memcpy(void* _dst, void* _src, u32 _size) {
    char *pDst = (char*)_dst;
    char *pSrc = (char*)_src;

    while (_size--) {
        *pDst++ = *pSrc++;
    }
}

#endif // _COMMON_H