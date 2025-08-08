#include "common.h"
#include "system/sound.h"
#include "psyq/kernel.h"

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", SoundInitialize);

void SoundReset(void) {
    int i;

    if (g_SoundControlFlags == 0x0) {
        SoundHandleError(0x29);
        return;
    }
    
    EnterCriticalSection();
    g_SoundControlFlags = 0;
    SpuSetIRQ(SPU_OFF);
    SpuSetTransferCallback(NULL);
    SpuSetIRQCallback(NULL);
    StopRCnt(RCntCNT2);
    CloseEvent(g_unk_SoundEvent);
    ExitCriticalSection();
    for (i = 0; i < NUM_VOICES; i++) {
        func_8003F5BC(i, 6, 3);
    }
    SoundSetVoiceKeyOff(0xFFFFFF); // Release all voices
    SpuSetReverbModeDepth(0, 0);
    SpuSetReverbModeType(SPU_REV_MODE_OFF);
    g_SoundSpuErrorId = 0;
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", SoundEnableAllSpuChannels);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", SoundMuteAllSpuChannels);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80037F44);

void func_80037F88(void) {
    if (g_SoundControlFlags & 1) {
        DisableEvent(g_unk_SoundEvent);
        g_SoundControlFlags &= ~1;
    }
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", SoundLoadWdsFile);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_800380D0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_800381F4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80038264);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003827C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80038310);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_800383EC);

void SoundAddSedsEntry(SoundFile* pSoundFile) {
    SoundFile* pEntry;
    short nSedsStatus;
    SoundFile** pList;
    SoundFile* pSoundFileToVerify = pSoundFile;

    // Ensure that an entry with the same SED ID does not exists in the linked list already
    if (!(g_SoundControlFlags & 0x80)) {
        for (pEntry = g_SoundSedsLinkedList; pEntry != NULL; pEntry = pEntry->pNext) {
            if (pSoundFile->sedId == pEntry->sedId) {
                SoundHandleError(SOUND_ERR_ENTRY_ALREADY_EXISTS);
                return;
            }
        }
    }

    // Validate SEDS File
    nSedsStatus = SoundValidateFile(pSoundFileToVerify, FILE_SIGNATURE('s','e','d','s'), 0x101);
    if (nSedsStatus != SOUND_STATUS_OK) {
        SoundHandleError(nSedsStatus);
        return;
    }

    // Add the SED Entry to the linked list
    DisableEvent(g_unk_SoundEvent);
    pList = &g_SoundSedsLinkedList;
    while (*pList != NULL)
        pList = &((*pList)->pNext);
    *pList = pSoundFile;
    pSoundFile->pNext = NULL;
    EnableEvent(g_unk_SoundEvent);
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003852C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80038624);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003864C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003869C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_800386C4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80038824);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003885C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_800388D4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003890C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80038934);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80038AD4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80038B4C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80038C68);

// TODO(jperos): CD Volume globals
INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80038D18);

void func_80038DB4(long reverb, long mix) {
    g_SoundSpuCommonAttr.cd.reverb = reverb;
    g_SoundSpuCommonAttr.cd.mix = mix;
    g_SoundSpuCommonAttr.mask = g_SoundSpuCommonAttr.mask | SPU_COMMON_CDREV | SPU_COMMON_CDMIX;
    SpuSetCommonAttr(&g_SoundSpuCommonAttr);
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80038DF4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80038E6C);
/*
Matches on GCC 2.7.2-970404 + ASPSX 2.63

void func_80038E6C(s16 volume, SpuVolume* pVolume, u16 arg2) {
    s16 var_v0;
    s32 temp_a2;

    pVolume->right = volume;
    pVolume->left = volume;
    
    if (g_SoundControlFlags & 0x600) {
        temp_a2 = arg2 & 0xFF;
        if (!(g_SoundControlFlags & 0x200)) {
            if ((temp_a2 ^ 1) != 0) {
                pVolume->left = -volume;
            } else {
                pVolume->right = -volume;
            }
            return;
        }

        if (temp_a2) {
            pVolume->left = -volume;
        } else {
            pVolume->right = -volume;
        }
    }  
}
*/

// Does not match on GCC 2.7.2-970404 + ASPSX 2.63, so likely start of a new TU here
void SoundHeapInitialize(void* startAddress, unsigned int size) {
    SoundHeapBlockHeader* pHeapBlock;
    unsigned int nAlignedSize;

    // Align start address and size
    pHeapBlock = (SoundHeapBlockHeader*) startAddress;
    nAlignedSize = size & ~0xF;
    if ((u32)pHeapBlock & 0xF) {
        nAlignedSize -= 0x10;
        pHeapBlock = ((u32)pHeapBlock + 0xF) & ~0xF;
    }
    
    g_SoundHeapEnd = (u32)pHeapBlock + nAlignedSize;
    pHeapBlock->unk0 = 0x8000;
    g_SoundHeapHead = pHeapBlock;
    g_SoundHeapSize = nAlignedSize;
    pHeapBlock->unk2 = 0;
    pHeapBlock->unk4 = 0;
    pHeapBlock->pPrev = pHeapBlock + 1;
    pHeapBlock->pNext = NULL;
}

void* SoundHeapAllocate(unsigned int allocSize) {
    void* pMemory;
    SoundHeapBlockHeader* pNewBlock;
    unsigned int nTotalSize;
    unsigned int nSize;
    SoundHeapBlockHeader* pNext;
    SoundHeapBlockHeader* pHeapBlock;

    DisableEvent(g_unk_SoundEvent);
    nTotalSize = ((allocSize + 0xF) & ~0xF) + sizeof(SoundHeapBlockHeader);
    
    for (pHeapBlock = g_SoundHeapHead; pHeapBlock->pNext != NULL; pHeapBlock = pHeapBlock->pNext) {
        pNext = pHeapBlock->pNext;
        nSize = (u32)pHeapBlock->pNext - (u32)pHeapBlock->pPrev;
        if (nSize >= nTotalSize) {
            goto alloc_new_block;
        }
    }
    
    pNext = (SoundHeapBlockHeader*)g_SoundHeapEnd;
    if ((u32)pNext - (u32)pHeapBlock->pPrev >= nTotalSize) {
    alloc_new_block:
        pNewBlock = (SoundHeapBlockHeader*)(((u32)pHeapBlock->pPrev + 0xF) & ~0xF);
        pMemory = pNewBlock + 1;
        pNewBlock->pPrev = (SoundHeapBlockHeader*)((u32)pMemory + allocSize);
        pNewBlock->pNext = NULL;
        pNewBlock->unk4 = 0;
        pNewBlock->unk0 = 2;
        pNewBlock->unk2 = 0;
        pNewBlock->pNext = pHeapBlock->pNext;
        pHeapBlock->pNext = pNewBlock;
        EnableEvent(g_unk_SoundEvent);
        SoundHeapClearBlockMemory(pMemory, allocSize);
        return pMemory;
    }
    
    return NULL;
}

// SoundHeapAllocate, but slightly different
INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80039024);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", SoundHeapFree);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_800391CC);

// SoundHeapSetBlockMemory (void* pBlockMemory, void* pSrc, int size)
INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", SoundHeapSetBlockMemory);

void SoundHeapClearBlockMemory(void* pMemory, int size) {
    unsigned int nCount;
    u32* pDword;
    u8* pByte;

    pDword = pMemory;

    for (nCount = size >> 4; nCount != 0; nCount--) {
        pDword[3] = 0;
        pDword[2] = 0;
        pDword[1] = 0;
        pDword[0] = 0;
        pDword += 4;
    }
    
    for (nCount = (size >> 2) & 3; nCount != 0; nCount--) {
        *pDword++ = 0;
    }
    
    pByte = (u8*) pDword;
    for (nCount = size & 3; nCount != 0; nCount--) {
        *pByte++ = 0;
    }
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80039360);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_800393B8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_800394B8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_800395B8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_800396E0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80039748);

s32 func_8003977C(void) { return 0; }

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80039784);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_800397C0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_800397FC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80039850);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80039910);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_800399D4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80039A80);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80039B68);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80039C4C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80039C8C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80039CC4);

void func_80039D24(void) {}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80039D2C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80039D78);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80039DB8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80039E18);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80039E60);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80039EC4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80039F18);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80039F9C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_80039FF8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003A094);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003A14C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003A20C);

void func_8003A2D4(void) {}

void func_8003A2DC(void) {}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003A2E4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003A344);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003A3B8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003A450);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003A4FC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003A55C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003A5D0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003A65C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003A82C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003A838);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003A89C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003A948);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003A9BC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003AA30);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003AAC4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003ABE8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003ABF0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003AC58);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003ACC8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003AD20);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003AD98);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003ADCC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003AE84);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003AF24);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003AFA0);

//----------------------------------------------------------------------------------------------------------------------
// NOTE(jperos): This matches on gcc 2.6.3 found here: https://decomp.me/scratch/hBvXX
void SoundAbortAllVoices(AudioManager* manager) {
    AudioElement* pElement;
    u32 cnt;

    cnt = manager->element_count;
    pElement = &manager->elements[0];

    do {

        if (pElement->active_flag) {
            SoundAbortVoiceOnChannel(&pElement->voice_data, pElement->voice_number);
        }
        pElement++;
        cnt--;
    } while (cnt);
}

//----------------------------------------------------------------------------------------------------------------------
void SoundReleaseAllVoices(AudioManager* manager) {
    AudioElement* pElement;
    u32 cnt;

    cnt = manager->element_count;
    pElement = &manager->elements[0];

    do {
        SoundReleaseVoiceFromChannel(&pElement->voice_data, pElement->voice_number);
        pElement++;
        cnt--;
    } while (cnt);
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003B0AC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003B148);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003B1FC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003B22C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003B32C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003B370);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003B424);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003B644);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003B930);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003B97C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003B9E4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003BA38);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003BB08);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003BB40);

void SoundOnTransferCallback(void) {
    SoundCommandCallback_t pCallback;

    pCallback = (&g_SoundTransferQueue[g_SoundTransferQueueReadIndex])->pCallbackFn;
    g_SoundControlFlags |= 4;
    if (pCallback) {
        pCallback();
    }
    
    g_SoundControlFlags &= 0xFFEF;
    if (g_SoundTransferQueueReadIndex != g_SoundTransferQueueWriteIndex) {
        SoundProcessTransferCommand();
    }
    g_SoundControlFlags &= 0xFFFB;
}

void SoundQueueSpuWriteCommand(u32 transferAddress, void* pData, u_long dataSize, SoundCommandCallback_t pCallback) {
    SoundQueueTransferCommand(transferAddress, pData, dataSize, pCallback, SOUND_SPU_COMMAND_WRITE);
}

void SoundQueueSpuReadCommand(u32 transferAddress, void* pData, u_long dataSize, SoundCommandCallback_t pCallback) {
    SoundQueueTransferCommand(transferAddress, pData, dataSize, pCallback, SOUND_SPU_COMMAND_READ);
}

// Queue SPU ReadDecodedData SPU_ALL Command
void func_8003BC58(u32 transferAddress, void* pData, u_long dataSize, SoundCommandCallback_t pCallback) {
    SoundQueueTransferCommand(transferAddress, pData, dataSize, pCallback, 3);
}

// Queue SPU ReadDecodedData SPU_CDONLY Command
void func_8003BC7C(u32 transferAddress, void* pData, u_long dataSize, SoundCommandCallback_t pCallback) {
    SoundQueueTransferCommand(transferAddress, pData, dataSize, pCallback, 4);
}

void SoundQueueTransferCommand(u32 transferAddress, void* pData, u_long dataSize, SoundCommandCallback_t pCallback, unsigned short commandType) {
    SoundTransferCommand* pCmd;
    unsigned short nControlFlags;
    unsigned short nNextIndex;

    nControlFlags = g_SoundControlFlags;
    if (!(nControlFlags & 4)) {
        while (SoundTransferQueueSync());
        EnterCriticalSection();
    }

    nNextIndex = g_SoundTransferQueueWriteIndex + 1;
    if (nNextIndex >= SOUND_TRANSFER_QUEUE_SIZE) {
        nNextIndex = 0;
    }
    g_SoundTransferQueueWriteIndex = nNextIndex;
    
    // TODO: pCmd = &g_SoundTransferQueue[nNextIndex]; doesn't match, but there should be a cleaner line here
    pCmd = nNextIndex * sizeof(SoundTransferCommand) + (u32)g_SoundTransferQueue;
    pCmd->commandType = commandType & 0xF;
    pCmd->unk2 = 0;
    pCmd->pSpuData = pData;
    pCmd->pTransferAddress = transferAddress & 0x7FFF8;
    pCmd->dataSize = dataSize;
    pCmd->pCallbackFn = pCallback;
    
    if (!(g_SoundControlFlags & 0x10)) {
        SoundProcessTransferCommand();
    }
    
    if (!(nControlFlags & 4)) {
        ExitCriticalSection();
    }
}

int SoundTransferQueueSync() {
    unsigned short nWriteIndex = g_SoundTransferQueueWriteIndex;
    if (nWriteIndex < g_SoundTransferQueueReadIndex) {
        nWriteIndex += SOUND_TRANSFER_QUEUE_SIZE;
    }
    return (nWriteIndex - g_SoundTransferQueueReadIndex < 6) ^ 1;
}

void func_8003BDF4(void) {}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003BDFC);

void SoundProcessTransferCommand(void) {
    SpuTransferCallbackProc pPrevCallback;
    unsigned short nNextIndex;
    SoundTransferCommand* pCmd;

    nNextIndex = g_SoundTransferQueueReadIndex + 1;
    if (nNextIndex >= SOUND_TRANSFER_QUEUE_SIZE) {
        nNextIndex = 0;
    }
    g_SoundTransferQueueReadIndex = nNextIndex;
    
    g_SoundControlFlags |= 0x10;

    // TODO: pCmd = &g_SoundTransferQueue[nNextIndex]; doesn't match, but there should be a cleaner line here
    pCmd = nNextIndex * sizeof(SoundTransferCommand) + (u32)g_SoundTransferQueue;
    
    pPrevCallback = SpuSetTransferCallback(&SoundOnTransferCallback);
    SpuSetTransferMode(SPU_TRANSFER_BY_DMA);
    SpuSetTransferStartAddr(pCmd->pTransferAddress);
    switch (pCmd->commandType) {
        case 0:
            break;
        case SOUND_SPU_COMMAND_WRITE:
            SpuWrite(pCmd->pSpuData, pCmd->dataSize);
            break;
        case SOUND_SPU_COMMAND_READ:
            SpuRead(pCmd->pSpuData, pCmd->dataSize);
            break;
        case 3:
            D_80059548 = SpuReadDecodedData(pCmd->pSpuData, SPU_ALL);
            break;
        case 4:
            D_80059548 = SpuReadDecodedData(pCmd->pSpuData, SPU_CDONLY);
            break;
    }
    
    if (pPrevCallback != &SoundOnTransferCallback) {
        SoundHandleError(SOUND_ERR_UNEXPECTED_CALLBACK);
    }
}

void SoundSpuIRQHandler(void) {
    g_SoundControlFlags |= SOUND_CTL_FLAG_IRQ_HANDLER;
    g_SoundSpuIRQCount++;
    if (g_SoundSpuIrqCallbackFn) {
        g_SoundSpuIrqCallbackFn();
    }
    g_SoundControlFlags &= ~SOUND_CTL_FLAG_IRQ_HANDLER;
}

void SoundSetSpuIrqCallback(u32 func) {
    g_SoundSpuIrqCallbackFn = func;
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003C020);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003C484);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003C4C4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003C6E8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003CC84);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003CD00);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003CD08);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003CD30);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003CD4C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003CD54);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003CD7C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003CD84);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003CD8C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003CE04);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003CE18);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003CE38);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003CE50);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003CE68);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003CE9C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003CEC0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003CED4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003CEF0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003CF38);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003CFA4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003CFF0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D034);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D070);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D0E8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D110);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D13C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D17C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D1BC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D208);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D21C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D298);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D2D0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D300);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D328);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D340);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D358);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D370);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D3A4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D3D8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D438);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D4A4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D4C4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D4E4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D53C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D59C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D5BC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D5C4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D5CC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D5D4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D60C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D640);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D65C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D678);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D694);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D6B4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D6D0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D6F8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D714);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D730);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D74C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D770);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D79C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D7C8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D7FC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D854);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D86C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D884);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D8B8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003D9A4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003DAB0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003DAEC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003DB0C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003DB2C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003DB58);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003DB98);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003DBE4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003DC50);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003DD24);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003DE18);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003DE54);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003DE74);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003DE94);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003DEB4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003DEE4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003DF3C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003DF78);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003E04C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003E140);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003E160);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003E180);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003E1F8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003E290);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003E308);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003E358);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003E360);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003E3E0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003E40C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003E44C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003E4BC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003E4F0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003E54C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003E5BC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003E680);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003E6C0);

void SoundClearVoiceDataPointers(void) {
    s32 offset = sizeof(SoundVoiceData*) * (NUM_VOICES - 1);
    while( offset >= 0 ) {
        *(u32*)((u8*)&g_SoundChannels + offset) = NULL;
        offset -= sizeof(SoundVoiceData*);
    }
}

//----------------------------------------------------------------------------------------------------------------------
void SoundAssignVoiceToChannelAndStop(SoundVoiceData* voiceData, u32 channelIndex) {

    SoundVoiceData* currentVoice;
    SoundVoiceData** pChannel;

    pChannel = &g_SoundChannels[channelIndex];
    if (channelIndex < NUM_VOICES) {
        currentVoice = *pChannel;

        // Mark current voice as needing update
        if (currentVoice == voiceData) {
            g_unk_VoicesNeedingProcessing = (1 << channelIndex) | g_unk_VoicesNeedingProcessing;
            return;
        }

        // Do not steal a higher priority voices
        if (currentVoice && currentVoice->priority > voiceData->priority) {
            return;
        }

        // Assign voice to channel
        voiceData->flags = 0xFFFF;
        voiceData->assignedVoice = channelIndex;
        g_SoundChannels[channelIndex] = voiceData;

        // Mark for voice processing
        g_unk_VoicesNeedingProcessing = (1 << channelIndex) | g_unk_VoicesNeedingProcessing;

        // Stop any current key ons for this channel
        g_SoundKeyOnFlags = ~(1 << channelIndex) & g_SoundKeyOnFlags;
    }
}

//----------------------------------------------------------------------------------------------------------------------
void SoundAssignVoiceToChannel(SoundVoiceData* voiceData, u32 channelIndex) {
    SoundVoiceData* currentVoice;
    SoundVoiceData** channelPtr;

    channelPtr = &g_SoundChannels[channelIndex];
    if (channelIndex < NUM_VOICES) {
        currentVoice = *channelPtr;

        if (currentVoice == voiceData || (currentVoice && currentVoice->priority > voiceData->priority)) {
            return;
        }

        voiceData->assignedVoice = channelIndex;
        *channelPtr = voiceData;
    }
}

//----------------------------------------------------------------------------------------------------------------------
void SoundReleaseVoiceFromChannel(SoundVoiceData* voiceData, uint channelIndex)
{
    SoundVoiceData** channelPtr;
    uint channelBitMask;

    channelPtr = &g_SoundChannels[channelIndex];
    if (channelIndex < NUM_VOICES) {
        if (*channelPtr == voiceData) {
            *channelPtr = NULL;
            channelBitMask = 1 << channelIndex;
            g_unk_VoicesNeedingProcessing = channelBitMask | g_unk_VoicesNeedingProcessing;

            // Clear key-on flag to prevent playback
            g_SoundKeyOnFlags = ~channelBitMask & g_SoundKeyOnFlags;
        }
    }
}

//----------------------------------------------------------------------------------------------------------------------
void SoundAbortVoiceOnChannel(SoundVoiceData* voiceData, u32 channelIndex) {

    if ((channelIndex < NUM_VOICES) && (g_SoundChannels[channelIndex] == voiceData)) {
        g_unk_VoicesNeedingProcessing = (1 << channelIndex) | g_unk_VoicesNeedingProcessing;
        g_SoundKeyOnFlags = ~(1 << channelIndex) & g_SoundKeyOnFlags;
    }
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003E900);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003EB5C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003EBF0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003EEA0);

//----------------------------------------------------------------------------------------------------------------------
void SoundAssignVoiceToChannelAndPlay(SoundVoiceData* voiceData, u32 channelIndex)
{
    SoundVoiceData* currentVoice;
    SoundVoiceData** pChannel;

    pChannel = &g_SoundChannels[channelIndex];
    if (channelIndex < NUM_VOICES) {
        currentVoice = *pChannel;

        // Skip assignment if voice already assigned
        if (currentVoice != voiceData) {
            if (currentVoice && currentVoice->priority > voiceData->priority) {
                return;
            }

            // Assign voice to channel
            voiceData->flags = 0xFFFF;
            voiceData->assignedVoice = channelIndex;
            g_SoundChannels[channelIndex] = voiceData;

            // Mark for voice processing
            g_unk_VoicesNeedingProcessing = (1 << channelIndex) | g_unk_VoicesNeedingProcessing;
        }

        // Always trigger playback on this channel
        g_SoundKeyOnFlags = (1 << channelIndex) | g_SoundKeyOnFlags;
    }
}

void SoundStopVoiceOnChannel(SoundVoiceData* voiceData, u32 channelIndex) {

    if( channelIndex < NUM_VOICES && g_SoundChannels[channelIndex] == voiceData ) {

        g_SoundKeyOffFlags = (1 << channelIndex) | g_SoundKeyOffFlags;
    }
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003EFE4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003F190);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003F1A4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003F1EC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003F240);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003F2A0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003F308);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003F354);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003F3C0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003F42C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003F43C);

void SoundSetVoiceKeyOn(u32 voiceFlags) {
    u16* pSoundRegisters = g_pSoundSpuRegisters;
    // TODO: Clean up this
    *(pSoundRegisters + 0xC4) = voiceFlags;
    *(pSoundRegisters + 0xC5) = (voiceFlags >> 0x10);
}

// Set the SPU_VOICE_KEY_OFF register, which will release / fade out voices according to the flags
void SoundSetVoiceKeyOff(u32 voiceFlags) {
    u16* pSoundRegisters = g_pSoundSpuRegisters;
    // TODO: Clean up this
    *(pSoundRegisters + 0xC6) = voiceFlags;
    *(pSoundRegisters + 0xC7) = (voiceFlags >> 0x10);
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", SoundSetReverbVoices);

void func_8003F4BC(void) {}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", SoundSetVoiceStartAddress);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", SoundSetVoiceLoopAddress);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", SoundSetVoiceVolume);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", SoundSetVoicePitch);

// ADSR Functions
INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003F530);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003F560);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003F588);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003F5BC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003F5EC);
// End ADSR functions

int SoundValidateFile(SoundFile* pSoundFile, u32 magicBytes, unsigned short targetValue) {
    unsigned char bIsError;
    
    if (pSoundFile->magic != magicBytes) {
        return SOUND_ERR_INVALID_SIGNATURE;
    }
    
    if (SoundFileComputeChecksum(pSoundFile) == 0) {
        // Version check?
        bIsError = (pSoundFile->unkC != targetValue);
        return bIsError * SOUND_ERR_UNK_0X4;
    }
    
    return SOUND_ERR_INVALID_CHECKSUM;
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", func_8003F67C);

int SoundFileComputeChecksum(SoundFile* pSoundFile) {
    int nResult;
    int* pCurrent;
    unsigned int nCount;

    pCurrent = pSoundFile;
    nCount = (pSoundFile->unk8 + 3) / 4; // Align to 4-byte boundary
    nResult = 0;
    do {
        nResult += *pCurrent++;
    } while (--nCount);
    
    return nResult;
}


INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/sound", SoundHandleError);
