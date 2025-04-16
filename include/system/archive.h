#ifndef _XENO_ARCHIVE_H
#define _XENO_ARCHIVE_H

#define ARCHIVE_HEADER_ENTRY_SIZE 0x7

extern u32 g_ArchiveTable;
extern u32 g_CurArchiveOffset;

int ArchiveDecodeSector(unsigned int entryIndex);

#endif