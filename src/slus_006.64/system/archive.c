#include "common.h"
#include "system/archive.h"
#include "psyq/pc.h"

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", ArchiveInit);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", func_800283D4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", ArchiveSetIndex);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", func_800284B4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", func_80028530);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", func_80028548);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", func_80028570);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", func_800286BC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", func_800286CC);

int ArchiveDecodeSize(unsigned int entryIndex) {
    char* pFilepath;
    int hArchiveFile;
    int nFileSize;
    u8* pArchiveEntry;
    u32 nOffset;
    
    u8 byte0;
    u8 byte1;
    u8 byte2;
    u8 byte3;
    
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

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", func_80028808);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", ArchiveDecodeAlignedSize);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", func_80028928);

char* ArchiveGetFilePath(unsigned int entryIndex) {
    u32 nOffset;
    
    if (D_8004FE48) {
        nOffset = (entryIndex + g_CurArchiveOffset - 1) * ARCHIVE_ENTRY_NAME_BUFFER_SIZE;
        return nOffset + D_8004FE48;
    }

    return NULL;
}

int ArchiveDecodeSector(unsigned int entryIndex) {
    u8* pArchiveEntry;
    u32 nOffset;

    nOffset = (entryIndex + g_CurArchiveOffset - 1) * ARCHIVE_HEADER_ENTRY_SIZE;
    pArchiveEntry = nOffset + g_ArchiveTable;
    return (pArchiveEntry[2] << 0x10) + (pArchiveEntry[1] << 0x8) + pArchiveEntry[0];
}
