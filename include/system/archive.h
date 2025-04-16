#ifndef _XENO_ARCHIVE_H
#define _XENO_ARCHIVE_H

#define ARCHIVE_HEADER_ENTRY_SIZE 0x7
#define ARCHIVE_ENTRY_NAME_BUFFER_SIZE 0x40

extern u32 D_8004FE48;
extern u32 g_ArchiveTable;
extern u32 g_CurArchiveOffset;

int ArchiveDecodeSize(unsigned int entryIndex);
char* ArchiveGetFilePath(unsigned int entryIndex);
int ArchiveDecodeSector(unsigned int entryIndex);

#endif