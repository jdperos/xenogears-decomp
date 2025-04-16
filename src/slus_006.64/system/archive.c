#include "common.h"
#include "system/archive.h"

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", ArchiveInit);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", func_800283D4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", ArchiveSetIndex);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", func_800284B4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", func_80028530);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", func_80028548);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", func_80028570);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", func_800286BC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", func_800286CC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", ArchiveDecodeSize);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", func_80028808);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", ArchiveDecodeAlignedSize);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", func_80028928);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/system/archive", func_80028998);

int ArchiveDecodeSector(unsigned int entryIndex) {
    u8* pArchiveEntry;
    u32 offset;

    offset = ((entryIndex + g_CurArchiveOffset - 1) * ARCHIVE_HEADER_ENTRY_SIZE);
    pArchiveEntry = offset + g_ArchiveTable;
    return (pArchiveEntry[2] << 0x10) + (pArchiveEntry[1] << 0x8) + pArchiveEntry[0];
}
