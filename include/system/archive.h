#ifndef _XENO_ARCHIVE_H
#define _XENO_ARCHIVE_H

#include "psyq/libcd.h"

#define ARCHIVE_HEADER_ENTRY_SIZE 0x7
#define ARCHIVE_ENTRY_NAME_BUFFER_SIZE 0x40
#define ARCHIVE_MAX_SECTIONS 0x40

#define ARCHIVE_CD_DRIVE_IDLE 0x0
#define ARCHIVE_CD_DRIVE_READ_SECTOR 0x1
#define ARCHIVE_CD_DRIVE_READ_DONE 0x2
#define ARCHIVE_CD_DRIVE_SEEK 0x3
#define ARCHIVE_CD_DRIVE_SEEK_DONE 0x4
#define ARCHIVE_CD_DRIVE_DONE 0x5
#define ARCHIVE_CD_DRIVE_READ_DATA_SECTOR 0x6
#define ARCHIVE_CD_DRIVE_HALT 0x7
#define ARCHIVE_CD_DRIVE_READ_ADPCM_SECTOR 0x8
#define ARCHIVE_CD_DRIVE_RESET_MODE 0x9
#define ARCHIVE_CD_DRIVE_ERROR 0xA
#define ARCHIVE_CD_DRIVE_HANDLE_ERROR 0xB
#define ARCHIVE_CD_DRIVE_SET_ADPCM_PLAY_CHANNEL 0xC

extern u32 D_8004FE48;
extern u32 g_ArchiveHeader;
extern u32 g_ArchiveTable;
extern u32 g_CurArchiveOffset;
extern int D_8004FE18;
extern u32 g_ArchiveCdDriveState;
extern CdlLOC g_ArchiveCdCurLocation;

s32 ArchiveSetIndex(int sectionIndex, int entryIndex);
int ArchiveGetArchiveOffsetIndices(int* alignedIndex, int* remainder);
int ArchiveDecodeSize(int entryIndex);
char* ArchiveGetFilePath(int entryIndex);
int ArchiveDecodeSector(int entryIndex);
void ArchiveCdSeekToFile(int entryIndex);
void ArchiveCdDriveCommandHandler(u_char status, u_char* pResult);

#endif