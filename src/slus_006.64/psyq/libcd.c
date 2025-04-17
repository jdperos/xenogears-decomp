#include "common.h"

extern int g_CdDebugLevel;

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CdInit);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_80040DA0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_80040DC8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_80040DF0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_80040E18);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_80040E38);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_80040E48);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_80040E58);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_80040E68);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_80040ED4);

int CdSetDebug(int level) {
    int prevLevel;
    
    prevLevel = g_CdDebugLevel;
    g_CdDebugLevel = level;
    return prevLevel;
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_80040F0C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_80040F40);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_80040F74);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_80040F94);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_80040FB4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_80040FCC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CdControl);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CdControlF);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_80041248);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_8004138C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_800413AC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_800413CC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_800413EC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CdDataSync);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_80041430);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_80041534);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_800415B4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CD_sync);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_80041DBC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CD_cw);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_800424A4);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CD_flush);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CD_initvol);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", func_80042700);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libcd", CD_init);
