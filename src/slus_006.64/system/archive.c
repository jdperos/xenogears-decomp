#include "common.h"
#include "system/archive.h"
#include "psyq/pc.h"
#include "psyq/libcd.h"

// Temp
extern int func_800286CC(void);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", ArchiveInit);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", func_800283D4);

int ArchiveSetIndex(int sectionIndex, int entryIndex) {
    s32 nOffset;

    nOffset = ((u_short*)g_ArchiveHeader)[sectionIndex + entryIndex] - 1;
    g_CurArchiveOffset = nOffset;
    if (nOffset < 0) {
        g_CurArchiveOffset = 0;
        return -1;
    }
    return nOffset;
}

int ArchiveGetArchiveOffsetIndices(int* alignedIndex, int* remainder) {
    int nAligned;
    int nIndex;
    u_short* pArchiveHeaderEntry;

    pArchiveHeaderEntry = g_ArchiveHeader;
    nIndex = 0;
    
    while(nIndex < ARCHIVE_MAX_SECTIONS) {
        if (*pArchiveHeaderEntry == g_CurArchiveOffset + 1) {
            // Round down to 4-byte boundary
            nAligned = (nIndex / 4) * 4;
            
            *alignedIndex = nAligned;
            *remainder = nIndex - nAligned;
            break;
        }
        
        nIndex += 1;
        pArchiveHeaderEntry += 1;
    }
    
    if (nIndex == ARCHIVE_MAX_SECTIONS) {
        *alignedIndex = 0;
        *remainder = 0;
    }
    
    return g_CurArchiveOffset;
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", func_80028530);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", func_80028548);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", func_80028570);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", func_800286BC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", func_800286CC);

int ArchiveDecodeSize(int entryIndex) {
    char* pFilepath;
    int hArchiveFile;
    int nFileSize;
    u8* pArchiveEntry;
    u32 nOffset;
    
    if (D_8004FE48) {
        pFilepath = ArchiveGetFilePath(entryIndex);
        hArchiveFile = PCopen(pFilepath, O_RDONLY, 0);
        nFileSize = PClseek(hArchiveFile, 0, SEEK_END);
        PCclose(hArchiveFile);
        if (nFileSize > 0) {
            goto exit;
        }
    }
    
    nOffset = (entryIndex + g_CurArchiveOffset - 1) * ARCHIVE_HEADER_ENTRY_SIZE;
    pArchiveEntry = nOffset + g_ArchiveTable;
    nFileSize = (
        (pArchiveEntry[6] << 0x18) + 
        (pArchiveEntry[5] << 0x10) + 
        (pArchiveEntry[4] << 0x8) + 
        pArchiveEntry[3]
    );

    exit:
    return nFileSize;
}

int ArchiveDecodeSizeAligned(int entryIndex) {
    char* pFilepath;
    int hArchiveFile;
    int nFileSize;
    u8* pArchiveEntry;
    u32 nOffset;
    
    if (D_8004FE48) {
        pFilepath = ArchiveGetFilePath(entryIndex);
        hArchiveFile = PCopen(pFilepath, O_RDONLY, 0);
        nFileSize = PClseek(hArchiveFile, 0, SEEK_END);
        PCclose(hArchiveFile);
        if (nFileSize > 0) {
            goto exit;
        }
    }
    
    nOffset = (entryIndex + D_8004FE18 - 1) * ARCHIVE_HEADER_ENTRY_SIZE;
    pArchiveEntry = nOffset + g_ArchiveTable;
    nFileSize = (
        (pArchiveEntry[6] << 0x18) + 
        (pArchiveEntry[5] << 0x10) + 
        (pArchiveEntry[4] << 0x8) + 
        pArchiveEntry[3]
    );

    exit:
    nFileSize = ((nFileSize + 3) / 4) * 4;
    return nFileSize;
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", ArchiveDecodeAlignedSize);
/*
Only matches on GCC 2.6.X

int ArchiveDecodeAlignedSize(unsigned int entryIndex) {
    s32 nSize;
    s32 nAlignedSize;
    
    nSize = ArchiveDecodeSize(entryIndex);
    nAlignedSize = ((nSize + 3 ) / 4);
    return nAlignedSize * 4;
}
*/

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", func_80028928);

char* ArchiveGetFilePath(int entryIndex) {
    u32 nOffset;
    
    if (D_8004FE48) {
        nOffset = (entryIndex + g_CurArchiveOffset - 1) * ARCHIVE_ENTRY_NAME_BUFFER_SIZE;
        return nOffset + D_8004FE48;
    }

    return NULL;
}

int ArchiveDecodeSector(int entryIndex) {
    u8* pArchiveEntry;
    u32 nOffset;

    nOffset = (entryIndex + g_CurArchiveOffset - 1) * ARCHIVE_HEADER_ENTRY_SIZE;
    pArchiveEntry = nOffset + g_ArchiveTable;
    return (pArchiveEntry[2] << 0x10) + (pArchiveEntry[1] << 0x8) + pArchiveEntry[0];
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", func_80028A18);

void ArchiveCdDataSync(int mode) {
    int nResult;

    if (mode == 0) {
        do {
            nResult = func_800286CC();
        } while (0 < nResult);
    }
    func_800286CC();
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", func_80028A94);
INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", func_80028AAC);
INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", func_80028B14);
INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", func_80028E60);
INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", func_80028ECC);
INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", func_80028F30);
INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", func_8002945C);
INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", func_800294B4);
INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", func_8002954C);
INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", func_800295D8);
INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", ArchiveReadFile);
INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", func_80029AFC);
INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", func_80029EB0);
INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", func_8002A260);
INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", func_8002A2D0);

void ArchiveCdSeekToFile(int entryIndex) {
    u_char nCommand;
    u_char* nParam;
    int nSector;
    CdlLOC* pCdLocation;

    if (entryIndex > 0) {
        pCdLocation = &g_ArchiveCdCurLocation;
        nSector = ArchiveDecodeSector(entryIndex);
        CdIntToPos(nSector, pCdLocation);
        g_ArchiveCdDriveState = ARCHIVE_CD_DRIVE_SEEK;
        CdSyncCallback(&ArchiveCdDriveCommandHandler);
        nCommand = CdlSetloc;
        nParam = &g_ArchiveCdCurLocation;
    } else {
        g_ArchiveCdDriveState = ARCHIVE_CD_DRIVE_DONE;
        CdSyncCallback(&ArchiveCdDriveCommandHandler);
        nCommand = CdlPause;
        nParam = NULL;
    }

    CdControlF(nCommand, nParam);
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", func_8002A428);
INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", func_8002A498);
INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", func_8002A524);
INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", func_8002A57C);
INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", ArchiveCdDriveCommandHandler);