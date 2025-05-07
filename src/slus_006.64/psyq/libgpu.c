#include "common.h"
#include "psyq/libgpu.h"


INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", SetDefDrawEnv);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", SetDefDispEnv);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", GetTexturePage);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", GetClut);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", func_80043A70);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", func_80043AD0);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", func_80043B10);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", func_80043B2C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", AddPrim);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", func_80043B84);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", func_80043BC0);

void TermPrim(void *p) {
    termPrim(p);
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", func_80043BFC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", func_80043C24);

void SetPolyF3(POLY_F3 *p) {
    setlen(p, 4);
    setcode(p, 0x20);
}

void SetPolyFT3(POLY_FT3 *p) {
    setlen(p, 7);
    setcode(p, 0x24);
}

void SetPolyG3(POLY_G3 *p) {
    setlen(p, 6);
    setcode(p, 0x30);
}

void SetPolyGT3(POLY_GT3 *p) {
    setlen(p, 9);
    setcode(p, 0x34);
}

void SetPolyF4(POLY_F4 *p) {
    setlen(p, 5);
    setcode(p, 0x28);
}

void SetPolyFT4(POLY_FT4 *p) {
    setlen(p, 9);
    setcode(p, 0x2C);
}

void SetPolyG4(POLY_G4 *p) {
    setlen(p, 8);
    setcode(p, 0x38);
}

void SetPolyGT4(POLY_GT4 *p) {
    setlen(p, 12);
    setcode(p, 0x3C);
}

void SetSprt8(SPRT_8 *p) {
    setlen(p, 3);
    setcode(p, 0x74);
}

void SetSprt16(SPRT_16 *p) {
    setlen(p, 3);
    setcode(p, 0x7C);
}

void SetSprt(SPRT *p) {
    setlen(p, 4);
    setcode(p, 0x64);
}

void SetTile1(TILE_1 *p) {
    setlen(p, 2);
    setcode(p, 0x68);
}

void SetTile8(TILE_8 *p) {
    setlen(p, 2);
    setcode(p, 0x70);
}

void SetTile16(TILE_16 *p) {
    setlen(p, 2);
    setcode(p, 0x78);
}

void SetTile(TILE *p) {
    setlen(p, 3);
    setcode(p, 0x60);
}

void SetLineF2(LINE_F2 *p) {
    setlen(p, 3);
    setcode(p, 0x40);
}

void SetLineG2(LINE_G2 *p) {
    setlen(p, 4);
    setcode(p, 0x50);
}

void SetLineF3(LINE_F3 *p) {
    setlen(p, 5);
    setcode(p, 0x48);
    p->pad = 0x55555555;
}

void SetLineG3(LINE_G3 *p) {
    setlen(p, 7);
    setcode(p, 0x58);
    p->pad = 0x55555555;
}

void SetLineF4(LINE_F4 *p) {
    setlen(p, 6);
    setcode(p, 0x4c);
    p->pad = 0x55555555;
}

void SetLineG4(LINE_G4 *p) {
    setlen(p, 9);
    setcode(p, 0x5c);
    p->pad = 0x55555555;
}

void SetDrawTPage(DR_TPAGE *p, int dfe, int dtd, int tpage) {
    setlen(p, 1);
    ((u_long *)(p))[1] = _get_mode(dfe, dtd, tpage);
}

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", func_80043E4C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", func_80043EAC);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", func_80043F18);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", func_80043F50);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", func_80044064);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", ResetGraph);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", func_80044294);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", SetGraphDebug);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", func_8004440C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", func_800444B8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", func_800444C8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", DrawSyncCallback);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", SetDispMask);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", DrawSync);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", func_8004463C);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", ClearImage);

// Clear Image, 2nd version
INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", func_800447F8);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", LoadImage);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", StoreImage);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", MoveImage);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", ClearOTag);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", ClearOTagR);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", DrawPrim);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", DrawOTag);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", PutDrawEnv);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", func_80044D48);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", func_80044E64);

INCLUDE_ASM("asm/slus_006.64/nonmatchings/psyq/libgpu", PutDispEnv);
