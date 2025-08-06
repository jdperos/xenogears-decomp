#include "common.h"
#include "psyq/libspu.h"
#include "psyq/libapi.h"
#include "psyq/stdarg.h"
#include "libdma.h"
#include "libmctrl.h"

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

void _spu_FiDMA(void); // Forward declare for SpuStart()
s32 _spu_t(s32, ...); // Forward declare for _spu_Fr and _spu_Fw

static inline void _memcpy(void* _dst, void* _src, u32 _size) {
    char *pDst = (char*)_dst;
    char *pSrc = (char*)_src;

    while (_size--) {
        *pDst++ = *pSrc++;
    }
}

void SpuInit(void) {
    _SpuInit(0);
}

void _SpuInit(s32 bHot) {
    s32 i;

    ResetCallback();
    _spu_init(bHot);

    if (bHot == 0)
    {
        for(i = 0; i < NUM_VOICES; i++)
        {
            _spu_voice_centerNote[i] = 0xC000;
        }
    }

    SpuStart();
    _spu_rev_flag = 0;
    _spu_rev_reserve_wa = 0;
    _spu_rev_attr.mode = 0;
    _spu_rev_attr.depth.left = 0;
    _spu_rev_attr.depth.right = 0;
    _spu_rev_attr.delay = 0;
    _spu_rev_attr.feedback = 0;
    _spu_rev_offsetaddr = _spu_rev_startaddr[0];
    _spu_FsetRXX(SPU_RXX_REV_WA_START_ADDR, _spu_rev_offsetaddr, 0);
    _spu_AllocBlockNum = 0;
    _spu_AllocLastNum = 0;
    _spu_memList = 0;
    _spu_trans_mode = 0;
    _spu_transMode = 0;
    _spu_keystat = 0;
    _spu_RQmask = 0;
    _spu_RQvoice = 0;
    _spu_env = 0;
}

void SpuStart(void) {
    s32 event;

    if (g_SpuRunning == 0) {
        g_SpuRunning = 1;
        EnterCriticalSection();
        _SpuDataCallback(_spu_FiDMA);
        // TODO: who knows what these constants do
        event = OpenEvent(0xF0000009U, 0x20, 0x2000, NULL);
        _spu_EVdma = event;
        EnableEvent(event);
        ExitCriticalSection();
    }
}

long _spu_init(long bHot) {
    u32 dmaTimer;
    int i;

    *_spu_sys_pcr |= DMA_DPCR_SPU_PRIORITY_HIGH | DMA_DPCR_MASK_DMA4_ENABLE;
    _spu_RXX->rxx.main_vol.left = 0;
    _spu_RXX->rxx.main_vol.right = 0;
    _spu_RXX->rxx.spucnt = 0;
    _spu_transMode = 0;
    _spu_addrMode = 0;
    _spu_tsa = 0;
    _spu_Fw1ts();
    _spu_RXX->rxx.main_vol.left = 0;
    _spu_RXX->rxx.main_vol.right = 0;

    dmaTimer = 0;
    while (_spu_RXX->rxx.spustat & 0x7FF)
    {
        if (++dmaTimer > DMA_TIMEOUT)
        {
            printf(D_8001946C, D_8001947C);
            break;
        }
    }

	// TODO(jperos): These are definitely macros
    _spu_mem_mode = 2;
    _spu_mem_mode_plus = 3;
    _spu_mem_mode_unit = 8;
    _spu_mem_mode_unitM = 7;
    _spu_RXX->rxx.data_trans = 4;

    _spu_RXX->rxx.rev_vol.left = 0;
    _spu_RXX->rxx.rev_vol.right = 0;
    _spu_RXX->rxx.key_off[0] = 0xFFFF;
    _spu_RXX->rxx.key_off[1] = 0xFFFF;
    _spu_RXX->rxx.rev_mode[0] = 0;
    _spu_RXX->rxx.rev_mode[1] = 0;

    for (i = 0; i < 10; i++)
    {
        _spu_RQ[i] = 0;
    }

    if (!bHot)
    {
        _spu_RXX->rxx.chan_fm[0] = 0;
        _spu_RXX->rxx.chan_fm[1] = 0;
        _spu_RXX->rxx.noise_mode[0] = 0;
        _spu_RXX->rxx.noise_mode[1] = 0;
        _spu_RXX->rxx.cd_vol.left = 0;
        _spu_RXX->rxx.cd_vol.right = 0;
        _spu_RXX->rxx.ex_vol.left = 0;
        _spu_RXX->rxx.ex_vol.right = 0;
        _spu_tsa = _SPU_DEFAULT_ADDR;

        _spu_FwriteByIO(&_spu_dummy, ARRAY_COUNT(_spu_dummy));

        for(i = 0; i < NUM_VOICES; i++)
        {
            _spu_RXX->rxx.voice[i].volume.left = 0; /* left volume */
            _spu_RXX->rxx.voice[i].volume.right = 0; /* right volume */
            _spu_RXX->rxx.voice[i].pitch = 0x3fff;  /* pitch */
            _spu_RXX->rxx.voice[i].addr = _SPU_DEFAULT_ADDR;  /* addr */
            _spu_RXX->rxx.voice[i].adsr[0] = 0; /* adsr1 */
            _spu_RXX->rxx.voice[i].adsr[1] = 0; /* adsr2 */
        }

        _spu_RXX->rxx.key_on[0] = 0xFFFF;
        _spu_RXX->rxx.key_on[1] = 0xFF;
        _spu_Fw1ts();
        _spu_Fw1ts();
        _spu_Fw1ts();
        _spu_Fw1ts();
        _spu_RXX->rxx.key_off[0] = 0xFFFF;
        _spu_RXX->rxx.key_off[1] = 0xFF;
        _spu_Fw1ts();
        _spu_Fw1ts();
        _spu_Fw1ts();
        _spu_Fw1ts();
    }

    _spu_inTransfer = 1;
    _spu_RXX->rxx.spucnt = SPU_CTRL_MASK_MUTE_SPU | SPU_CTRL_MASK_SPU_ENABLE;
    _spu_transferCallback = NULL;
    _spu_IRQCallback = NULL;
    return 0;
}

static void _spu_FwriteByIO(void* data, u32 size) {
    u16 initStatus;
    s32 dmaTimer;
    s32 blockSize;
    u16 spucnt;
    u16* data16 = data;

	// TODO(jperos): 0x7FF is the 11 lowest bits. Should macro this in a way that makes sense for whatever this represents
    initStatus = _spu_RXX->rxx.spustat & 0x7FF;
    _spu_RXX->rxx.trans_addr = _spu_tsa;
    _spu_Fw1ts();

    while (size != 0)
    {
		// TODO(jperos): I've seen 0x40 in a few other transfers - is this max chunk size?
        if (size <= 0x40)
        {
            blockSize = size;
        }
        else
        {
            blockSize = 0x40;
        }

        dmaTimer = 0;
        while (dmaTimer < blockSize)
        {
            _spu_RXX->rxx.trans_fifo = *data16++;
             dmaTimer += 2;
        }

        spucnt = _spu_RXX->rxx.spucnt;
        spucnt &= ~SPU_CTRL_TRANSFER_MODE_DMA_READ;
        spucnt |= SPU_CTRL_TRANSFER_MODE_MANUAL_WRITE;
        _spu_RXX->rxx.spucnt = spucnt;
        _spu_Fw1ts();

        dmaTimer = 0;
        while (_spu_RXX->rxx.spustat & SPU_STAT_MASK_DATA_TRANSFER_BUSY)
        {
            if (++dmaTimer > DMA_TIMEOUT)
            {
                printf(D_8001946C, D_8001948C);
                break;
            }
        }

        _spu_Fw1ts();
        _spu_Fw1ts();

        size -= blockSize;
    }

    spucnt = _spu_RXX->rxx.spucnt;
    spucnt &= ~SPU_CTRL_TRANSFER_MODE_DMA_READ;
    spucnt |= SPU_CTRL_TRANSFER_MODE_STOP;
    _spu_RXX->rxx.spucnt = spucnt;

    dmaTimer = 0;
    while ((_spu_RXX->rxx.spustat & 0x7FF) != initStatus)
    {
        if (++dmaTimer > DMA_TIMEOUT)
        {
            printf(D_8001946C, D_800194A0);
            break;
        }
    }
}

void _spu_FiDMA(void) {
    s32 i;

    if (_spu_dma_mode == SPU_DMA_MODE_WRITE)
    {
        _spu_Fw1ts();
    }

    _spu_RXX->rxx.spucnt &= ~SPU_CTRL_TRANSFER_MODE_DMA_READ;

    for(i = 0; _spu_RXX->rxx.spucnt & SPU_CTRL_TRANSFER_MODE_DMA_READ; )
    {
        if (++i > DMA_TIMEOUT) {
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
    _spu_RXX->rxx.spucnt |= SPU_CTRL_TRANSFER_MODE_DMA_READ;
    _spu_Fw1ts();
    _spu_FsetDelayR();
    *_spu_madr = madr;
    *_spu_bcr = (bcr << 16) | 16;
    _spu_dma_mode = SPU_DMA_MODE_READ;
    *_spu_chcr = DMA_CHCR_SPU_READ;
}


s32 _spu_t(s32 operation, ...) {
    u16 spuTransferMode;
    s32 chcr;
    u32 dmaTimer;
    u32 addr;
    va_list args;
    u32 arg;
    u16 controlRegister;

    va_start(args, operation);

    switch (operation) {
        case SPU_DMA_CMD_SETADDR:
            arg = va_arg(args, u32);
            _spu_RXX->rxx.trans_addr = _spu_tsa = arg >> _spu_mem_mode_plus;
            break;
        case SPU_DMA_CMD_WRITE:
            _spu_dma_mode = SPU_DMA_MODE_WRITE;

            dmaTimer = 0;
            while (_spu_RXX->rxx.trans_addr != _spu_tsa) {
                if (++dmaTimer > DMA_TIMEOUT) {
                    return DMA_TRANSFER_TIMEOUT;
                }
            }

            // _spu_RXX->rxx.spucnt = (_spu_RXX->rxx.spucnt & 0xFFCF) | 0x20;
            controlRegister = _spu_RXX->rxx.spucnt;
            controlRegister &= ~SPU_CTRL_TRANSFER_MODE_DMA_READ;
            controlRegister |= SPU_CTRL_TRANSFER_MODE_DMA_WRITE;
            _spu_RXX->rxx.spucnt = controlRegister;
            break;
        case SPU_DMA_CMD_READ:
            _spu_dma_mode = SPU_DMA_MODE_READ;

            dmaTimer = 0;
            while (_spu_RXX->rxx.trans_addr != _spu_tsa) {
                if (++dmaTimer > DMA_TIMEOUT) {
                    return DMA_TRANSFER_TIMEOUT;
                }
            }

            // _spu_RXX->rxx.spucnt |= 0x30;
            controlRegister = _spu_RXX->rxx.spucnt;
            controlRegister &= ~SPU_CTRL_TRANSFER_MODE_DMA_READ;
            controlRegister |= SPU_CTRL_TRANSFER_MODE_DMA_READ;
            _spu_RXX->rxx.spucnt = controlRegister;
            break;
        case SPU_DMA_CMD_EXEC:
            if (_spu_dma_mode == SPU_DMA_MODE_READ) {
                spuTransferMode = SPU_CTRL_TRANSFER_MODE_DMA_READ;
            } else {
                spuTransferMode = SPU_CTRL_TRANSFER_MODE_DMA_WRITE;
            }

            dmaTimer = 0;
            while ((_spu_RXX->rxx.spucnt & SPU_CTRL_TRANSFER_MODE_DMA_READ) != spuTransferMode) {
                if (++dmaTimer > DMA_TIMEOUT) {
                    return DMA_TRANSFER_TIMEOUT;
                }
            }

            if (_spu_dma_mode == SPU_DMA_MODE_READ) {
                _spu_FsetDelayR();
            } else {
                _spu_FsetDelayW();
            }

            arg = va_arg(args, u32);
            _spu_transfer_startaddr = arg;
            arg = va_arg(args, u32);
            _spu_transfer_time = (arg / DMA_BLOCK_SIZE);
            _spu_transfer_time += ((arg % DMA_BLOCK_SIZE) ? 1 : 0);
            *_spu_madr = _spu_transfer_startaddr;
            *_spu_bcr = (_spu_transfer_time << 16) | 16;
            if (_spu_dma_mode == SPU_DMA_MODE_READ) {
                chcr = DMA_CHCR_SPU_READ;
            } else {
                chcr = DMA_CHCR_SPU_WRITE;
            }
            *_spu_chcr = chcr;
            break;
    }

    return DMA_TRANSFER_SUCCESS;
}

s32 _spu_Fw(s32 addr, s32 size) {
    if (_spu_transMode == SPU_TRANSFER_BY_DMA) {
        _spu_t(SPU_DMA_CMD_SETADDR, _spu_tsa << _spu_mem_mode_plus);
        _spu_t(SPU_DMA_CMD_WRITE);
        _spu_t(SPU_DMA_CMD_EXEC, addr, size);
    }
    else
    {
        _spu_FwriteByIO(addr, size);
    }
    return size;
}

s32 _spu_Fr(s32 addr, s32 size) {
    _spu_t(SPU_DMA_CMD_SETADDR, _spu_tsa << _spu_mem_mode_plus);
    _spu_t(SPU_DMA_CMD_READ);
    _spu_t(SPU_DMA_CMD_EXEC, addr, size);
    return size;
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

// Set Register Address (value)
// Note: Addresses are stored in SPU RAM as 16 bit values, as 1/8th ( >> 3 ) versions of the actual memory address
u32 _spu_FsetRXXa(s32 offset, u32 value) {
    u32 translated_address;

    // Align the address value to 8 bits
    if ((_spu_mem_mode != 0) && ((value % _spu_mem_mode_unit) != 0)) {
        value += _spu_mem_mode_unit;
        value &= ~_spu_mem_mode_unitM;
    }

    // Shift it to translate it to the actual SPU-ranged value
    translated_address = value >> _spu_mem_mode_plus;
    switch (offset) {
        case SPU_ADDRESS_MODE_SPU:
            return translated_address & 0xFFFF;
        case SPU_ADDRESS_MODE_ALIGNED:
            return value;
        default:
            _spu_RXX->raw[offset] = translated_address;
    }

    return value;
}

// Get Register Address (value)
// Note: Addresses are stored in SPU RAM as 16 bit values, as 1/8th ( >> 3 ) versions of the actual memory address
s32 _spu_FgetRXXa(s32 offset, s32 addressing_mode) {
    u16 value;

    value = _spu_RXX->raw[offset];
    switch (addressing_mode) {
        case SPU_ADDRESS_MODE_SPU:
            return value;
        default:
            return value << _spu_mem_mode_plus;
    }
}

void _spu_FsetPCR(s32 bKindOfHighPriority) {
    *_spu_sys_pcr &= ~DMA_DPCR_MASK_DMA4_PRIORITY;
    if (bKindOfHighPriority) {
        *_spu_sys_pcr |= (DMA_PRIORITY_HIGH << DMA_DPCR_DMA4_PRIORITY_SHIFT);
    } else {
        *_spu_sys_pcr |= (DMA_PRIORITY_MEDIUM << DMA_DPCR_DMA4_PRIORITY_SHIFT);
    }
}

// NOTE(jperos): I'm currently unaware of exactly what these DMA timing override codes mean exactly.
//               From: https://psx-spx.consoledev.net/memorycontrol/ :
//                   1F801014h - SPU Delay/Size (200931E1h) (use 220931E1h for SPU-RAM reads)
static void _spu_FsetDelayW(void) {
    *_spu_delay = (*_spu_delay & ~MCTRL_DELAY_DMA_TIMING_OVERRIDE_MASK) |
                  MCTRL_DELAY_DMA_TIMING_SELECT;
}

static void _spu_FsetDelayR(void) {
    *_spu_delay = (*_spu_delay & ~MCTRL_DELAY_DMA_TIMING_OVERRIDE_MASK) |
                  ((2 << MCTRL_DELAY_DMA_TIMING_OVERRIDE_SHIFT) | MCTRL_DELAY_DMA_TIMING_SELECT);
}

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
        CloseEvent(_spu_EVdma);
        DisableEvent(_spu_EVdma);
        ExitCriticalSection();
    }
}

// TODO(jperos): This all compiles, but I am currently unsure as to the significance of these flags. Current best guesses here
#define SPU_MALLOC_UNK_FLAG_1      (1 << 28)
#define SPU_MALLOC_UNK_FLAG_2      (1 << 29)
#define SPU_MALLOC_END_MARKER      (1 << 30)
#define SPU_MALLOC_SKIP_ENTRY      (1 << 31)
#define SPU_MALLOC_FLAGS           (SPU_MALLOC_UNK_FLAG_1 | SPU_MALLOC_UNK_FLAG_2 | SPU_MALLOC_END_MARKER | SPU_MALLOC_SKIP_ENTRY)
#define SPU_MALLOC_ADDR_MASK       (~SPU_MALLOC_FLAGS)

long SpuInitMalloc(long num, char *top) {
    if (num > 0) {
        ((SPU_MALLOC *)top)->addr = SPU_MALLOC_END_MARKER | SPU_RESERVED_TOTAL;
        ((SPU_MALLOC *)top)->size = (_SPU_RAM_SIZE << _spu_mem_mode_plus) - SPU_RESERVED_TOTAL;
        _spu_memList = top;
        _spu_AllocLastNum = 0;
        _spu_AllocBlockNum = num;
        return num;
    }

    return 0;
}

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

long SpuSetReverb (long on_off)
{
    long spuControlRegister;

    switch( on_off ) {
    case SPU_OFF:
        _spu_rev_flag = SPU_OFF;
        spuControlRegister = _spu_RXX->_rxx.spucnt;
        spuControlRegister &= ~SPU_CTRL_MASK_REVERB_MASTER_ENABLE;
        _spu_RXX->_rxx.spucnt = spuControlRegister;
        break;
        
    case SPU_ON:
        if( _spu_rev_reserve_wa != on_off && _SpuIsInAllocateArea_(_spu_rev_offsetaddr) ) {
            _spu_rev_flag = SPU_OFF;
            spuControlRegister = _spu_RXX->_rxx.spucnt;
            spuControlRegister &= ~SPU_CTRL_MASK_REVERB_MASTER_ENABLE;
            _spu_RXX->_rxx.spucnt = spuControlRegister;
        } else {
            _spu_rev_flag = on_off;
            spuControlRegister = _spu_RXX->_rxx.spucnt;
            spuControlRegister |= SPU_CTRL_MASK_REVERB_MASTER_ENABLE;
            _spu_RXX->_rxx.spucnt = spuControlRegister;
        }
        break;
    }

    return _spu_rev_flag;
}

long _SpuIsInAllocateArea(u_long addr) {
    SPU_MALLOC* mem_entry;
    long index;
    SPU_MALLOC* mem_list = (SPU_MALLOC*)_spu_memList;

    if (_spu_memList == NULL) {
        return 0;
    }

    for (index = 0; 1; index++) {
        mem_entry = &mem_list[index];

        if (mem_entry->addr & SPU_MALLOC_SKIP_ENTRY) continue;

        if (mem_entry->addr & SPU_MALLOC_END_MARKER) break;

        if ((mem_entry->addr & SPU_MALLOC_ADDR_MASK) >= addr) {
            return 1;
        }

        if (((mem_entry->addr & SPU_MALLOC_ADDR_MASK) + mem_entry->size) > addr) {
            return 1;
        }
    }

    return 0;
}

// The same as _SpuIsInAllocateArea, but with an SPU-ranged memory address
long _SpuIsInAllocateArea_(long addr) {
    SPU_MALLOC* mem_entry;
    long index;
    SPU_MALLOC* mem_list = (SPU_MALLOC*)_spu_memList;

    addr <<= _spu_mem_mode_plus;

    if (_spu_memList == NULL) {
        return 0;
    }

    for (index = 0; 1; index++) {
        mem_entry = &mem_list[index];

        if (mem_entry->addr & SPU_MALLOC_SKIP_ENTRY) continue;

        if (mem_entry->addr & SPU_MALLOC_END_MARKER) break;

        if ((mem_entry->addr & SPU_MALLOC_ADDR_MASK) >= addr) {
            return 1;
        }

        if (((mem_entry->addr & SPU_MALLOC_ADDR_MASK) + mem_entry->size) > addr) {
            return 1;
        }
    }

    return 0;
}

// TODO(jperos): Replace these constants as soon as the docs make sense
long SpuReadDecodedData(SpuDecodedData *d_data, long flag) {
    switch (flag) {
        case SPU_CDONLY:
            _spu_Fr_(d_data->cd_left, 0, 0x20);
            break;
        case SPU_VOICEONLY:
            _spu_Fr_(d_data->voice1, 0x100, 0x20);
            break;
        default:
            _spu_Fr_(d_data->cd_left, 0, 0x40);
            break;
    }

    if (_spu_RXX->rxx.spustat & SPU_STAT_CAPTURE_SECONDHALF) {
        return SPU_DECODE_SECONDHALF;
    } else {
        return SPU_DECODE_FIRSTHALF;
    }
}

long SpuSetIRQ(long on_off) {
    u_long dmaTimer;

    if ((on_off == SPU_OFF) || (on_off == SPU_RESET)) {
        _spu_RXX->rxx.spucnt &= ~SPU_STAT_MASK_IRQ9_FLAG;
        dmaTimer = 0;
        while (_spu_RXX->rxx.spucnt & SPU_STAT_MASK_IRQ9_FLAG) {
            if (++dmaTimer > DMA_TIMEOUT) {
                printf(D_800194B4, D_800194C4);
                return SPU_ERROR;
            }
        }

    }
    if ((on_off == SPU_ON) || (on_off == SPU_RESET)) {
        _spu_RXX->rxx.spucnt |= SPU_STAT_MASK_IRQ9_FLAG;
        dmaTimer = 0;
        while ( !(_spu_RXX->rxx.spucnt & SPU_STAT_MASK_IRQ9_FLAG) ) {
            if (++dmaTimer > DMA_TIMEOUT) {
                printf(D_800194B4, D_800194D4);
                return SPU_ERROR;
            }
        }
    }
    return on_off;
}

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

void SpuGetVoiceEnvelopeAttr(int vNum, long* keyStat, short* envx) {
    u_short volx;
    long offset;

    offset = (vNum * SPU_VOICE_REG_SIZE) + SPU_VOICE_REG_VOLUMEX;
    volx = _spu_RXX->raw[offset];

    *envx = volx;
    if (_spu_keystat & (1 << vNum)) {
        if ((volx << 16) > 0) {
            *keyStat = SPU_ON;
        } else {
            *keyStat = SPU_ON_ENV_OFF;
        }
    } else if ((volx << 16) > 0) {
        *keyStat = SPU_OFF_ENV_ON;
    } else {
        *keyStat = SPU_OFF;
    }
}

u_long SpuRead(u_char* addr, u_long size) {

    if (size > SPU_MAX_TRANSFER_SIZE) {
        size = SPU_MAX_TRANSFER_SIZE;
    }
    _spu_Fr(addr, size);
    if (_spu_transferCallback == 0) {
        _spu_inTransfer = 0;
    }
    return size;
}

u_long SpuWrite(u_char *addr, u_long size) {
    if (size > SPU_MAX_TRANSFER_SIZE) {
        size = SPU_MAX_TRANSFER_SIZE;
    }

    _spu_Fw(addr, size);
    if (_spu_transferCallback == NULL) {
        _spu_inTransfer = 0;
    }

    return size;
}

u_long SpuSetTransferStartAddr(u_long addr) {
    u32 offset;
    u16 base_addr;

    offset = addr - SPU_MIN_ADDR;
    if (offset > SPU_MAX_VALID_OFFSET) {
        return 0;
    }

    base_addr = _spu_FsetRXXa(SPU_ADDRESS_MODE_SPU, addr);
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
    _spu_trans_mode = mode;
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

long SpuSetReverbModeType(long mode) {
    ReverbPreset preset;
    s32 bClearWorkArea;
    u32 bMasterReverbEnableFlag;
    u16 spucnt;

    bClearWorkArea = 0;

    if (mode & SPU_REV_MODE_CLEAR_WA) {
        mode &= ~SPU_REV_MODE_CLEAR_WA;
        bClearWorkArea = 1;
    }

    if (mode < 0 || mode >= SPU_REV_MODE_MAX) {
        return SPU_ERROR;
    }

    if (_SpuIsInAllocateArea_(_spu_rev_startaddr[mode])) {
        return SPU_ERROR;
    }

    _spu_rev_attr.mode = mode;
    _spu_rev_offsetaddr = _spu_rev_startaddr[mode];

    _memcpy(&preset, &_spu_rev_param[mode], sizeof(ReverbPreset));

    preset.m_Mask = 0;
    switch (mode) {
        case SPU_REV_MODE_ECHO:
            _spu_rev_attr.feedback = 0x7F;
            _spu_rev_attr.delay = 0x7F;
            break;
        case SPU_REV_MODE_DELAY:
            _spu_rev_attr.feedback = 0;
            _spu_rev_attr.delay = 0x7F;
            break;
        default:
            _spu_rev_attr.feedback = 0;
            _spu_rev_attr.delay = 0;
            break;
    }

    bMasterReverbEnableFlag = (_spu_RXX->rxx.spucnt & SPU_CTRL_MASK_REVERB_MASTER_ENABLE) != 0;
    if (bMasterReverbEnableFlag) {
        spucnt = _spu_RXX->rxx.spucnt;
        spucnt &= ~SPU_CTRL_MASK_REVERB_MASTER_ENABLE;
        _spu_RXX->rxx.spucnt = spucnt;
    }

    _spu_RXX->rxx.rev_vol.left = 0;
    _spu_RXX->rxx.rev_vol.right = 0;
    _spu_rev_attr.depth.left = 0;
    _spu_rev_attr.depth.right = 0;

    _spu_setReverbAttr(&preset);

    if (bClearWorkArea) {
        SpuClearReverbWorkArea(mode);
    }

    _spu_FsetRXX(SPU_RXX_REV_WA_START_ADDR, _spu_rev_offsetaddr, 0);

    if (bMasterReverbEnableFlag) {
        spucnt = _spu_RXX->rxx.spucnt;
        spucnt |= SPU_CTRL_MASK_REVERB_MASTER_ENABLE;
        _spu_RXX->rxx.spucnt = spucnt;
    }
    return SPU_SUCCESS;
}

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

// TODO(jperos): The significance of these constants is yet not understood by me
long SpuClearReverbWorkArea (long mode) {
    volatile s32 callback;
    s32 oldTransmode;
    s32 addr;
    s32 bTransferring;
    s32 bTansmodeCleared;
    u32 size;
    u32 remaining;

    callback = 0;
    bTansmodeCleared = 0;

    if ((mode >= (u_long)SPU_REV_MODE_MAX) ||
        (_SpuIsInAllocateArea_(_spu_rev_startaddr[mode]))) {
        return SPU_ERROR;
    }

    if (mode == SPU_REV_MODE_OFF) {
        remaining = 0x10 << _spu_mem_mode_plus;
        addr = 0xFFF0 << _spu_mem_mode_plus;
    } else {
        remaining = (_SPU_RAM_SIZE - _spu_rev_startaddr[mode]) << _spu_mem_mode_plus;
        addr = _spu_rev_startaddr[mode] << _spu_mem_mode_plus;
    }

    oldTransmode = _spu_transMode;
    if (_spu_transMode == 1) {
        _spu_transMode = 0;
        bTansmodeCleared = 1;
    }

    bTransferring = 1;
    if (_spu_transferCallback != 0) {
        callback = _spu_transferCallback;
        _spu_transferCallback = 0;
    }

    while (bTransferring) {
        size = remaining;
        if (!(remaining < (0x400U + 1))) {
            size = 0x400;
        } else {
            bTransferring = 0;
        }
        _spu_t(SPU_DMA_CMD_SETADDR, addr);
        _spu_t(SPU_DMA_CMD_WRITE);
        _spu_t(SPU_DMA_CMD_EXEC, &_spu_zerobuf, size);
        WaitEvent(_spu_EVdma);
        remaining -= 0x400;
        addr += 0x400;
    };

    if (bTansmodeCleared) {
        _spu_transMode = oldTransmode;
    }

    if (callback) {
        _spu_transferCallback = callback;
    }

    return SPU_SUCCESS;
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libspu", WaitEvent);

void SpuSetReverbModeDepth(short DepthL, short DepthR)
{
    short* RL = &_spu_RXX->_rxx.rev_vol.left;
    short* RR = &_spu_RXX->_rxx.rev_vol.right;
    *RL = DepthL;
    *RR = DepthR;
    _spu_rev_attr.depth.left = DepthL;
    _spu_rev_attr.depth.right = DepthR;
}

void SpuSetReverbModeDelayTime(long delay) {
    ReverbPreset preset;
    s32 scaledDelay;
    s32 scaledDelay2;

    if (_spu_rev_attr.mode > SPU_REV_MODE_DELAY) {
        return;
    }

    if (_spu_rev_attr.mode < SPU_REV_MODE_ECHO) {
        return;
    }

    _memcpy(&preset, &_spu_rev_param[_spu_rev_attr.mode], sizeof(ReverbPreset));

    preset.m_Mask = SPU_REV_MASK_mLSAME
        | SPU_REV_MASK_mRSAME
        | SPU_REV_MASK_mLCOMB1
        | SPU_REV_MASK_dLSAME
        | SPU_REV_MASK_mLAPF1
        | SPU_REV_MASK_mRAPF1;

    scaledDelay  = (delay << 12) / 127;  /* delay * 4096 / 127 */
    scaledDelay2 = (delay << 13) / 127;  /* delay * 8192 / 127 */

    preset.m_Regs.m_mLSAME  = scaledDelay2 - preset.m_Regs.m_dAPF1;
    preset.m_Regs.m_mRSAME  = scaledDelay  - preset.m_Regs.m_dAPF2;
    preset.m_Regs.m_mLCOMB1 = preset.m_Regs.m_mRCOMB1 + scaledDelay;
    preset.m_Regs.m_dLSAME  = preset.m_Regs.m_dRSAME + scaledDelay;
    preset.m_Regs.m_mLAPF1  = preset.m_Regs.m_mLAPF2 + scaledDelay;
    preset.m_Regs.m_mRAPF1  = preset.m_Regs.m_mRAPF2 + scaledDelay;

    _spu_rev_attr.delay = delay;
    _spu_setReverbAttr(&preset);
}

void SpuSetReverbModeFeedback(long feedback) {
    ReverbPreset preset;

    if (_spu_rev_attr.mode > SPU_REV_MODE_DELAY) {
        return;
    }

    if (_spu_rev_attr.mode < SPU_REV_MODE_ECHO) {
        return;
    }

    _memcpy(&preset, &_spu_rev_param[_spu_rev_attr.mode], sizeof(ReverbPreset));

    _spu_rev_attr.feedback = feedback;
    preset.m_Mask = SPU_REV_MASK_vWALL;
    preset.m_Regs.m_vWALL = (feedback * 0x8100) / 127;
    _spu_setReverbAttr(&preset);
}

void SpuGetReverbModeType(long* type)
{
    *type = _spu_rev_attr.mode;
}